#include <math.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  HAS_MOVE_TO_DO,
  NOT_MOVE_TO_DO,
  MOVEMENT_CANCELED
} isr_state_t;

class StepperMotor {
 public:
  StepperMotor(
      gpio_num_t step_pin_number,
      gpio_num_t direction_pin_number,
      gpio_num_t enable_pin_number,
      int microsteps,
      float step,
      float reduction,
      uint16_t frequency = 10 * 1000,
      mcpwm_unit_t mcpwm_unit = MCPWM_UNIT_0,
      mcpwm_timer_t mcpwm_timer = MCPWM_TIMER_0,
      mcpwm_generator_t mcpwm_generator = MCPWM_GEN_A,
      mcpwm_io_signals_t mcpwm_io_signals = MCPWM_CAP_0);

  void begin();
  bool real_isr_handler();
  static bool isr_handler(mcpwm_unit_t mcpwm, mcpwm_capture_channel_id_t cap_sig, const cap_event_data_t* edata, void* arg);
  void move(uint16_t time_in_seconds, float final_angle);
  void cancele_movement();

 private:
  isr_state_t isr_movement_state;
  SemaphoreHandle_t wait_isr_semaphore;

  gpio_num_t step_pin;
  gpio_num_t direction_pin;
  gpio_num_t enable_pin;

  mcpwm_unit_t mcpwm_unit;
  mcpwm_timer_t mcpwm_timer;
  mcpwm_generator_t mcpwm_generator;
  mcpwm_io_signals_t mcpwm_io_signals;

  int8_t current_direction;
  int8_t microsteps;
  int32_t current_step;
  uint16_t frequency;
  float step;
  float reduction;
  float angle_per_pulse;

  volatile int32_t p;
  volatile uint32_t dx;
  volatile uint32_t dy;
  volatile uint32_t y;
};

#ifdef __cplusplus
}
#endif
