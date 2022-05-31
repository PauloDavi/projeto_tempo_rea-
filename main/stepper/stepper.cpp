#include "stepper.h"

static mcpwm_dev_t* MCPWM[2] = {&MCPWM0, &MCPWM1};
int counter = 0;
bool led_state = false;

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

  this->wait_isr_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(this->wait_isr_semaphore);

  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_2, 1);

  gpio_set_direction(this->step_pin, GPIO_MODE_OUTPUT);
  gpio_set_direction(this->direction_pin, GPIO_MODE_OUTPUT);
  gpio_set_direction(this->enable_pin, GPIO_MODE_OUTPUT);
}

void Stepper::begin() {
  mcpwm_gpio_init(this->mcpwm_unit, this->mcpwm_io_signals_pwm, this->step_pin);

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 10 * 1000;
  pwm_config.cmpr_a = 0;
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
  counter++;
  mcpwm_ll_intr_clear_timer_tez_status(MCPWM[this->mcpwm_unit], 1);

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if (this->isr_is_running == NOT_MOVE_TO_DO) {
    return;
  }

  uint32_t duty = 0;

  this->x++;
  if (this->p < 0) {
    this->p = this->p + this->twoDy;
  } else {
    this->y++;
    duty = mcpwm_ll_timer_get_peak(MCPWM[this->mcpwm_unit], this->mcpwm_timer, false) * 50 / 100;
    this->p = this->p - this->twoDyDx;
  }

  mcpwm_ll_operator_set_compare_value(MCPWM[this->mcpwm_unit], this->mcpwm_timer, this->mcpwm_io_signals_pwm, duty);

  if (this->y == this->dy) {
    this->isr_is_running = NOT_MOVE_TO_DO;

    mcpwm_ll_operator_set_compare_value(MCPWM[this->mcpwm_unit], this->mcpwm_timer, this->mcpwm_io_signals_pwm, 0);
    xSemaphoreGiveFromISR(this->wait_isr_semaphore, &xHigherPriorityTaskWoken);
  }

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Stepper::move(int initial_step, int delta_time, int final_step) {
  if (xSemaphoreTake(this->wait_isr_semaphore, (TickType_t)portMAX_DELAY) == pdTRUE) {
    printf("Move to %d\n", counter);
    this->dx = delta_time;
    printf("Move to %d\n", counter);

    this->dy = abs(initial_step - final_step);
    printf("Move to %d\n", counter);
    this->p = 2 * this->dy - this->dx;
    printf("Move to %d\n", counter);
    this->twoDy = 2 * this->dy;
    printf("Move to %d\n", counter);
    this->twoDyDx = 2 * (this->dy - this->dx);
    printf("Move to %d\n", counter);
    this->x = 0;
    printf("Move to %d\n", counter);
    this->y = initial_step;
    printf("Move to %d\n", counter);

    printf("Finish calc");
    this->isr_is_running = HAS_MOVE_TO_DO;
    printf("Move to %d\n", counter);
  }
}
