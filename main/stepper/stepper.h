#include <math.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "hal/mcpwm_ll.h"
#include "hal/mcpwm_types.h"
#include "soc/mcpwm_periph.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "soc/rtc.h"

static mcpwm_dev_t* MCPWM[2] = {&MCPWM0, &MCPWM1};

typedef enum {
  STEPPER_MOTOR_STOPPED,
  STEPPER_MOTOR_ACCELERATING,
  STEPPER_MOTOR_AT_SPEED,
  STEPPER_MOTOR_DECELERATING,
  STEPPER_MOTOR_STOPPING
} stepper_status_t;

typedef enum {
  STEPPER_MOTOR_DIRECTION_FORWARD,
  STEPPER_MOTOR_DIRECTION_BACKWARD,
} stepper_direction_t;

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

  mcpwm_unit_t mcpwm_unit = MCPWM_UNIT_0;
  mcpwm_timer_t mcpwm_timer = MCPWM_TIMER_0;
  mcpwm_io_signals_t mcpwm_io_signals_pwm = MCPWM0A;

  stepper_status_t motor_status = STEPPER_MOTOR_STOPPED;
  stepper_direction_t direction = STEPPER_MOTOR_DIRECTION_FORWARD;

  uint32_t duty_in_50_per_100;

  int current_step = 0;
  int current_direction = 1;
  int microsteps;
  float step;
  float reduction;
  float angle_per_pulse;
  float current_angle = 0;
  uint32_t frequency = 1000;

  volatile int p;
  volatile int dx;
  volatile int dy;
  volatile int y;
};
