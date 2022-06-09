#include "stepper.h"

Stepper::Stepper(
    gpio_num_t step_pin_number,
    gpio_num_t direction_pin_number,
    gpio_num_t enable_pin_number,
    int microsteps,
    float step,
    float reduction) {
  this->step_pin = step_pin_number;
  this->direction_pin = direction_pin_number;
  this->enable_pin = enable_pin_number;
  this->microsteps = microsteps;
  this->step = step;
  this->reduction = reduction;
  this->isr_is_running = NOT_MOVE_TO_DO;
  this->angle_per_pulse = step / microsteps / reduction;
  this->duty_in_50_per_100 = mcpwm_ll_timer_get_peak(MCPWM[this->mcpwm_unit], this->mcpwm_timer, false) * 50 / 100;

  this->wait_isr_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(this->wait_isr_semaphore);

  gpio_config_t io_conf = {};
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask =
      (1ULL << this->step_pin) |
      (1ULL << this->direction_pin) |
      (1ULL << this->enable_pin);

  gpio_config(&io_conf);
}

void Stepper::begin() {
  mcpwm_gpio_init(this->mcpwm_unit, this->mcpwm_io_signals_pwm, this->step_pin);

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 10 * 1000;
  pwm_config.cmpr_a = 50;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  mcpwm_init(this->mcpwm_unit, this->mcpwm_timer, &pwm_config);
  mcpwm_isr_register(this->mcpwm_unit, this->isr_handler, this, ESP_INTR_FLAG_IRAM, NULL);
  mcpwm_ll_intr_enable_timer_tez(MCPWM[this->mcpwm_unit], this->mcpwm_timer, true);
}

void IRAM_ATTR Stepper::isr_handler(void* arg) {
  Stepper* stepper = (Stepper*)arg;
  stepper->real_isr_handler();
}

void Stepper::real_isr_handler() {
  mcpwm_ll_intr_clear_timer_tez_status(MCPWM[this->mcpwm_unit], 1);

  if (this->isr_is_running == NOT_MOVE_TO_DO) {
    return;
  }

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  uint32_t duty = 0;

  this->p += this->dy;

  if (this->p > 0) {
    this->y++;
    this->current_step += current_direction;
    this->p -= this->dx;

    duty = this->duty_in_50_per_100;
  }

  mcpwm_ll_operator_set_compare_value(MCPWM[this->mcpwm_unit], this->mcpwm_timer, this->mcpwm_io_signals_pwm, duty);

  if (this->y == this->dy) {
    this->isr_is_running = NOT_MOVE_TO_DO;

    mcpwm_ll_operator_set_compare_value(MCPWM[this->mcpwm_unit], this->mcpwm_timer, this->mcpwm_io_signals_pwm, 0);
    xSemaphoreGiveFromISR(this->wait_isr_semaphore, &xHigherPriorityTaskWoken);
  }

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Stepper::move(int time_in_seconds, float final_angle) {
  if (xSemaphoreTake(this->wait_isr_semaphore, (TickType_t)portMAX_DELAY) == pdTRUE) {
    float current_angle = this->current_step * this->angle_per_pulse;

    if (current_angle >= final_angle) {
      gpio_set_level(this->direction_pin, 1);
      this->current_direction = -1;
    } else {
      gpio_set_level(this->direction_pin, 0);
      this->current_direction = 1;
    }

    this->dx = time_in_seconds * 10000;
    this->dy = int((abs(current_angle - final_angle) / this->angle_per_pulse) + 0.5);

    this->p = -this->dx / 2;

    this->y = 0;

    this->isr_is_running = HAS_MOVE_TO_DO;
  }
}
