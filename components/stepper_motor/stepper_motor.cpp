#include "stepper_motor.h"

StepperMotor::StepperMotor(
    gpio_num_t step_pin_number,
    gpio_num_t direction_pin_number,
    gpio_num_t enable_pin_number,
    int microsteps,
    float step,
    float reduction,
    uint16_t frequency,
    mcpwm_unit_t mcpwm_unit,
    mcpwm_timer_t mcpwm_timer,
    mcpwm_generator_t mcpwm_generator,
    mcpwm_io_signals_t mcpwm_io_signals) {
  this->step_pin = step_pin_number;
  this->direction_pin = direction_pin_number;
  this->enable_pin = enable_pin_number;
  this->microsteps = microsteps;
  this->step = step;
  this->reduction = reduction;
  this->isr_is_running = NOT_MOVE_TO_DO;
  this->angle_per_pulse = step / (microsteps / reduction);
  this->frequency = frequency;
  this->mcpwm_unit = mcpwm_unit;
  this->mcpwm_timer = mcpwm_timer;
  this->mcpwm_generator = mcpwm_generator;
  this->mcpwm_io_signals = mcpwm_io_signals;

  this->wait_isr_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(this->wait_isr_semaphore);

  gpio_config_t io_conf = {};
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask =
      (1ULL << this->step_pin) |
      (1ULL << this->direction_pin) |
      (1ULL << this->enable_pin);

  ESP_ERROR_CHECK(gpio_config(&io_conf));
}

void StepperMotor::begin() {
  ESP_ERROR_CHECK(mcpwm_gpio_init(this->mcpwm_unit, this->mcpwm_io_signals, this->step_pin));

  mcpwm_config_t pwm_config;
  pwm_config.frequency = this->frequency;
  pwm_config.cmpr_a = 0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  ESP_ERROR_CHECK(mcpwm_init(this->mcpwm_unit, this->mcpwm_timer, &pwm_config));

  mcpwm_capture_config_t isr_pwm_config;
  isr_pwm_config.cap_edge = MCPWM_POS_EDGE;
  isr_pwm_config.cap_prescale = 1;
  isr_pwm_config.capture_cb = this->isr_handler;
  isr_pwm_config.user_data = this;

  ESP_ERROR_CHECK(mcpwm_capture_enable_channel(this->mcpwm_unit, MCPWM_SELECT_CAP0, &isr_pwm_config));
}

bool IRAM_ATTR StepperMotor::isr_handler(
    mcpwm_unit_t mcpwm,
    mcpwm_capture_channel_id_t cap_sig,
    const cap_event_data_t* edata,
    void* arg) {
  StepperMotor* stepper_motor = (StepperMotor*)arg;

  return stepper_motor->real_isr_handler();
}

bool StepperMotor::real_isr_handler() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if (this->isr_is_running == NOT_MOVE_TO_DO) {
    return xHigherPriorityTaskWoken == pdTRUE;
  }

  if (this->isr_is_running == MOVEMENT_CANCELED) {
    mcpwm_set_duty(this->mcpwm_unit, this->mcpwm_timer, this->mcpwm_generator, 0);
    this->isr_is_running = NOT_MOVE_TO_DO;
    xSemaphoreGiveFromISR(this->wait_isr_semaphore, &xHigherPriorityTaskWoken);
    return xHigherPriorityTaskWoken == pdTRUE;
  }

  uint32_t duty = 0;

  this->p += this->dy;

  if (this->p > 0) {
    this->y++;
    this->current_step += current_direction;
    this->p -= this->dx;

    duty = 50;
  }

  mcpwm_set_duty(this->mcpwm_unit, this->mcpwm_timer, this->mcpwm_generator, duty);

  if (this->y == this->dy) {
    this->isr_is_running = NOT_MOVE_TO_DO;

    mcpwm_set_duty(this->mcpwm_unit, this->mcpwm_timer, this->mcpwm_generator, 0);
    xSemaphoreGiveFromISR(this->wait_isr_semaphore, &xHigherPriorityTaskWoken);
  }

  return xHigherPriorityTaskWoken == pdTRUE;
}

void StepperMotor::move(uint16_t time_in_seconds, float final_angle) {
  if (xSemaphoreTake(this->wait_isr_semaphore, (TickType_t)portMAX_DELAY) == pdTRUE) {
    float current_angle = this->current_step * this->angle_per_pulse;

    if (current_angle >= final_angle) {
      ESP_ERROR_CHECK(gpio_set_level(this->direction_pin, 1));
      this->current_direction = -1;
    } else {
      ESP_ERROR_CHECK(gpio_set_level(this->direction_pin, 0));
      this->current_direction = 1;
    }

    this->dx = time_in_seconds * this->frequency;
    this->dy = uint32_t((abs(current_angle - final_angle) / this->angle_per_pulse) + 0.5);

    this->p = -this->dx / 2;

    this->y = 0;

    this->isr_is_running = HAS_MOVE_TO_DO;
  }
}
