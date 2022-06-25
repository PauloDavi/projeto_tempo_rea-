#include "encoder.h"

Encoder::Encoder(
    gpio_num_t pin_data,
    gpio_num_t pin_clk,
    gpio_num_t pin_btn,
    QueueHandle_t keyboard_event_queue,
    uint16_t frequency,
    uint16_t min_time_press_button_ms) {
  this->pin_data = pin_data;
  this->pin_clk = pin_clk;
  this->pin_btn = pin_btn;
  this->keyboard_event_queue = keyboard_event_queue;
  this->frequency = frequency;
  this->min_time_press_button_ms = min_time_press_button_ms;
  this->times_to_press_button = (this->min_time_press_button_ms * this->frequency) / 1000;
  this->btn_counter = 0;
  this->clk_state = 0;

  ESP_LOGI("TAG", "Inicio");

  ESP_ERROR_CHECK(gpio_set_pull_mode(this->pin_btn, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_direction(this->pin_btn, GPIO_MODE_INPUT));

  ESP_ERROR_CHECK(gpio_set_pull_mode(this->pin_data, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_direction(this->pin_data, GPIO_MODE_INPUT));

  ESP_ERROR_CHECK(gpio_set_pull_mode(this->pin_clk, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_direction(this->pin_clk, GPIO_MODE_INPUT));

  this->btn_state = gpio_get_level(this->pin_btn);
  ESP_LOGI("TAG", "GPIO");

  // timer_config_t timer_config = {
  //     .alarm_en = TIMER_ALARM_EN,
  //     .counter_en = TIMER_PAUSE,
  //     .intr_type = TIMER_INTR_LEVEL,
  //     .counter_dir = TIMER_COUNT_UP,
  //     .auto_reload = TIMER_AUTORELOAD_MAX,
  //     .divider = uint32_t(APB_CLK_FREQ / this->frequency),
  // };

  // ESP_ERROR_CHECK(timer_init(this->timer_group, this->timer_id, &timer_config));
  // ESP_ERROR_CHECK(timer_set_counter_value(this->timer_group, this->timer_id, 0));
  // ESP_ERROR_CHECK(timer_set_alarm_value(this->timer_group, this->timer_id, 1));
  // ESP_ERROR_CHECK(timer_isr_callback_add(this->timer_group, this->timer_id, isr_handler, this, 0));
  // ESP_ERROR_CHECK(timer_enable_intr(this->timer_group, this->timer_id));

  ESP_LOGI("TAG", "Inicio timer");

  gptimer_config_t timer_config;
  timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
  timer_config.direction = GPTIMER_COUNT_UP;
  timer_config.resolution_hz = this->frequency;

  gptimer_alarm_config_t alarm_config;
  alarm_config.reload_count = 0;
  alarm_config.alarm_count = 1;
  alarm_config.flags.auto_reload_on_alarm = true;

  gptimer_event_callbacks_t cbs;
  cbs.on_alarm = this->isr_handler;

  ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &(this->gptimer)));
  ESP_ERROR_CHECK(gptimer_set_alarm_action(this->gptimer, &alarm_config));
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(this->gptimer, &cbs, this));
  ESP_LOGI("TAG", "timer");
}

void Encoder::init() {
  ESP_ERROR_CHECK(gptimer_enable(this->gptimer));
  ESP_ERROR_CHECK(gptimer_start(this->gptimer));
}

bool IRAM_ATTR Encoder::isr_handler(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) {
  Encoder *encoder = (Encoder *)user_data;
  return encoder->real_isr_handler();
}

bool Encoder::real_isr_handler() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  int btn_current_state = gpio_get_level(this->pin_btn);

  if (btn_current_state == 0) {
    this->btn_counter++;
  }

  if (btn_current_state != this->btn_state) {
    if (this->btn_counter >= this->times_to_press_button) {
      key_events_t event = ENCODER_BUTTON_CLICK_EVENT;
      xQueueSendFromISR(this->keyboard_event_queue, &event, &xHigherPriorityTaskWoken);
    }
    this->btn_counter = 0;
  }

  this->btn_state = btn_current_state;

  this->clk_state = this->clk_state >> 2;

  if (gpio_get_level(this->pin_clk)) {
    this->clk_state |= 0x4;
  }
  if (gpio_get_level(this->pin_data)) {
    this->clk_state |= 0x8;
  }

  if ((0x2814 >> this->clk_state) & 0x1) {
    this->clk_counter++;
  } else if ((0x4182 >> this->clk_state) & 0x1) {
    this->clk_counter--;
  }

  if (this->clk_counter == 4) {
    key_events_t event = ENCODER_CLOCKWISE_EVENT;
    xQueueSendFromISR(this->keyboard_event_queue, &event, &xHigherPriorityTaskWoken);
    this->clk_counter = 0;
  } else if (this->clk_counter == -4) {
    key_events_t event = ENCODER_ANTICLOCKWISE_EVENT;
    xQueueSendFromISR(this->keyboard_event_queue, &event, &xHigherPriorityTaskWoken);
    this->clk_counter = 0;
  }

  return xHigherPriorityTaskWoken;
}
