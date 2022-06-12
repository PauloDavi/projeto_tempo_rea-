#include <math.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/mcpwm.h"
// #include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "hal/mcpwm_ll.h"

static mcpwm_dev_t* MCPWM[2] = {&MCPWM0, &MCPWM1};

typedef enum {
  HAS_MOVE_TO_DO,
  NOT_MOVE_TO_DO,
} isr_state_t;

class Stepper {
 public:
  Stepper(
      gpio_num_t step_pin_number,
      gpio_num_t direction_pin_number,
      gpio_num_t enable_pin_number,
      int microsteps,
      float step,
      float reduction);

  void begin();
  void real_isr_handler();
  static void isr_handler(void* arg);
  void move(int delta_time, float final_step);

 private:
  isr_state_t isr_is_running;
  SemaphoreHandle_t wait_isr_semaphore;

  gpio_num_t step_pin;
  gpio_num_t direction_pin;
  gpio_num_t enable_pin;

  mcpwm_unit_t mcpwm_unit;
  mcpwm_timer_t mcpwm_timer;
  mcpwm_io_signals_t mcpwm_io_signals_pwm;

  uint32_t duty_in_50_per_100;

  int current_step;
  int current_direction;
  int microsteps;
  float step;
  float reduction;
  float angle_per_pulse;
  uint32_t frequency;

  volatile int p;
  volatile int dx;
  volatile int dy;
  volatile int y;
};
