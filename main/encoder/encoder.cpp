#include "encoder.h"

int counter = 0;

Encoder::Encoder(gpio_num_t pin_data, gpio_num_t pin_clk, gpio_num_t pin_btn) {
  this->pin_data = pin_data;
  this->pin_clk = pin_clk;
  this->pin_btn = pin_btn;
  this->position = 0;

  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  gpio_pad_select_gpio(this->pin_btn);
  ESP_ERROR_CHECK(gpio_set_pull_mode(this->pin_btn, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_direction(this->pin_btn, GPIO_MODE_INPUT));
  ESP_ERROR_CHECK(gpio_set_intr_type(this->pin_btn, GPIO_INTR_NEGEDGE));

  gpio_pad_select_gpio(this->pin_data);
  ESP_ERROR_CHECK(gpio_set_pull_mode(this->pin_data, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_direction(this->pin_data, GPIO_MODE_INPUT));

  gpio_pad_select_gpio(this->pin_clk);
  ESP_ERROR_CHECK(gpio_set_pull_mode(this->pin_clk, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_direction(this->pin_clk, GPIO_MODE_INPUT));
  ESP_ERROR_CHECK(gpio_set_intr_type(this->pin_clk, GPIO_INTR_POSEDGE));
}

void IRAM_ATTR button_isr_handler(void *) {
  counter++;
}

void Encoder::init() {
  ESP_ERROR_CHECK(gpio_isr_handler_add(this->pin_btn, button_isr_handler, NULL));
  ESP_ERROR_CHECK(gpio_isr_handler_add(this->pin_clk, isr_handler, this));
}

void IRAM_ATTR Encoder::isr_handler(void *arg) {
  Encoder *encoder = (Encoder *)arg;
  encoder->real_isr_handler();
}

int Encoder::get_counter() {
  return counter;
}

void Encoder::real_isr_handler() {
  if (gpio_get_level(this->pin_data)) {
    --(this->position);
  } else {
    ++(this->position);
  }
}

void Encoder::rotary_encoder_reset() {
  this->position = 0;
}
