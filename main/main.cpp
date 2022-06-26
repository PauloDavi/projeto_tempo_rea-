#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include "display_control.h"
#include "driver/gpio.h"
#include "encoder.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "fontx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "ili9340.h"
#include "pngle.h"
#include "stepper_motor.h"

#define FONT_COLOR 0x0004

static const char *TAG = "CPM Fisiotech";

void encoder_task(void *arg) {
  QueueHandle_t keyboard_event_queue = xQueueCreate(16, sizeof(key_events_t));
  key_events_t event;
  display_control_t display_control;

  Encoder encoder(
      GPIO_NUM_32,
      GPIO_NUM_33,
      GPIO_NUM_35,
      keyboard_event_queue);

  encoder.init();

  initDisplay(&display_control, FONT_COLOR);

  for (;;) {
    if (xQueueReceive(keyboard_event_queue, &event, (TickType_t)portMAX_DELAY) == pdPASS) {
      switch (event) {
        case ENCODER_BUTTON_CLICK_EVENT:
          switch (display_control.current_display) {
            case START_SCREEN:
              start_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            case ANGLE_SCREEN:
              angle_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            case DURATION_SCREEN:
              duration_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            case VELOCITY_SCREEN:
              velocity_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            case SUMMARY_SCREEN:
              summary_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            case WARNING_SCREEN:
              warning_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            case SECTION_SCREEN:
              section_screen_action_button_click(&display_control, FONT_COLOR);
              break;
            default:
              break;
          }
          break;

        case ENCODER_CLOCKWISE_EVENT:
          switch (display_control.current_display) {
            case ANGLE_SCREEN:
              angle_screen_action_clockwise(&display_control, FONT_COLOR);
              break;
            case DURATION_SCREEN:
              duration_screen_action_clockwise(&display_control, FONT_COLOR);
              break;
            case VELOCITY_SCREEN:
              velocity_screen_action_clockwise(&display_control, FONT_COLOR);
              break;
            case SUMMARY_SCREEN:
              summary_screen_action_clockwise(&display_control, FONT_COLOR);
              break;
            case WARNING_SCREEN:
              warning_screen_action_clockwise(&display_control, FONT_COLOR);
              break;
            default:
              break;
          }
          break;

        case ENCODER_ANTICLOCKWISE_EVENT:
          switch (display_control.current_display) {
            case ANGLE_SCREEN:
              angle_screen_action_anti_clockwise(&display_control, FONT_COLOR);
              break;
            case DURATION_SCREEN:
              duration_screen_action_anti_clockwise(&display_control, FONT_COLOR);
              break;
            case VELOCITY_SCREEN:
              velocity_screen_action_anti_clockwise(&display_control, FONT_COLOR);
              break;
            case SUMMARY_SCREEN:
              summary_screen_action_anti_clockwise(&display_control, FONT_COLOR);
              break;
            case WARNING_SCREEN:
              warning_screen_action_anti_clockwise(&display_control, FONT_COLOR);
              break;
            default:
              break;
          }
      }
    }
  }
}

void stepper_move(void *arg) {
  StepperMotor stepperMotor(
      GPIO_NUM_5,
      GPIO_NUM_18,
      GPIO_NUM_19,
      32,
      1.8,
      4.07);

  stepperMotor.begin();

  for (;;) {
    stepperMotor.move(10, 359);
    vTaskDelay(1000 * 2 / portTICK_PERIOD_MS);

    stepperMotor.move(30, 0);
    vTaskDelay(1000 * 2 / portTICK_PERIOD_MS);
  }
}

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Initializing SPIFFS");

  esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 16,
      .format_if_mount_failed = true};

  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
    } else {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(NULL, &total, &used);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
  } else {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }

  xTaskCreate(encoder_task, "encoder", 1024 * 8, NULL, 5, NULL);
  xTaskCreate(stepper_move, "stepper_move", 1024 * 2, NULL, 10, NULL);
}
