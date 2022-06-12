#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_timer.h"

class Encoder {
 public:
  Encoder(gpio_num_t pin_data, gpio_num_t pin_clk, gpio_num_t pin_btn);

  void rotary_encoder_reset();
  void real_isr_handler();
  int get_counter();
  void init();
  static void isr_handler(void* arg);

  int32_t position;
  gpio_num_t pin_data, pin_clk, pin_btn;
};
