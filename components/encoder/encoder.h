#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef enum {
  ENCODER_BUTTON_CLICK_EVENT,
  ENCODER_CLOCKWISE_EVENT,
  ENCODER_ANTICLOCKWISE_EVENT
} key_events_t;

class Encoder {
 public:
  Encoder(
      gpio_num_t pin_data,
      gpio_num_t pin_clk,
      gpio_num_t pin_btn,
      QueueHandle_t keyboard_event_queue,
      uint16_t frequency = 10000,
      uint16_t min_time_press_button_ms = 20);

  void init();

 private:
  static bool isr_handler(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data);
  bool real_isr_handler();

  uint8_t clk_state;
  int8_t clk_counter;
  bool btn_state;
  uint16_t btn_counter;
  uint16_t frequency;
  uint16_t times_to_press_button;
  uint16_t min_time_press_button_ms;

  QueueHandle_t keyboard_event_queue;
  gpio_num_t pin_data, pin_clk, pin_btn;
  gptimer_handle_t gptimer;
};
