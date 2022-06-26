#include "display_control.h"
#include "encoder.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "fontx.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "ili9340.h"
#include "stepper_motor.h"

#define FONT_COLOR 0x0004

// static const char *TAG = "CPM Fisiotech";

struct move_event_t {
  uint8_t angle;
  uint8_t duration;
  uint8_t velocity;
};
QueueHandle_t move_event_queue = xQueueCreate(1, sizeof(move_event_t));
static TaskHandle_t concel_movement_task_handle = NULL;

void concel_movement_task(void *arg) {
  StepperMotor *stepper_motor = (StepperMotor *)arg;

  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    stepper_motor->cancele_movement();
  }
}

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
              switch (display_control.current_state) {
                case BACK_BUTTON_ACTIVE:
                  display_control.current_state = BACK_BUTTON_ACTIVE;
                  display_control.current_display = START_SCREEN;
                  display_control.angle = 0;
                  display_control.duration = 0;
                  display_control.velocity = 1;
                  lcdDrawStartScreen(&display_control, FONT_COLOR);
                  break;
                case NEXT_BUTTON_ACTIVE:
                  display_control.current_state = BACK_BUTTON_ACTIVE;
                  display_control.current_display = COUNTDOWN_SCREEN;
                  lcdDrawCountdownScreen(&display_control, FONT_COLOR);

                  move_event_t move_event;
                  move_event.angle = display_control.angle;
                  move_event.duration = display_control.duration;
                  move_event.velocity = display_control.velocity;
                  xQueueSend(move_event_queue, (void *)&move_event, (TickType_t)0);
                  break;
                default:
                  break;
              }
              break;
            case SECTION_SCREEN:
              xTaskNotifyGive(concel_movement_task_handle);
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
  move_event_t move_event;
  StepperMotor stepperMotor(
      GPIO_NUM_5,
      GPIO_NUM_18,
      GPIO_NUM_19,
      32,
      1.8,
      4.07);
  xTaskCreate(concel_movement_task, "concel_movement_task", 200, &stepperMotor, 7, &concel_movement_task_handle);

  stepperMotor.begin();

  for (;;) {
    if (xQueueReceive(move_event_queue, &move_event, (TickType_t)portMAX_DELAY) == pdPASS) {
      uint16_t move_time_in_seconds = move_event.duration * 60;
      uint16_t move_quantity = ((move_time_in_seconds * move_event.velocity / move_event.angle) + 0.5);
      uint16_t time_of_move = ((move_time_in_seconds / move_quantity) + 0.5);

      for (uint16_t i = 0; i < move_quantity; i++) {
        stepperMotor.move(time_of_move, move_event.angle);
        stepperMotor.move(time_of_move, 0);
      }
    }
  }
}

extern "C" void app_main(void) {
  esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 16,
      .format_if_mount_failed = true};

  if (esp_vfs_spiffs_register(&conf) != ESP_OK) {
    return;
  }

  xTaskCreate(encoder_task, "encoder", 1024 * 8, NULL, 5, NULL);
  xTaskCreate(stepper_move, "stepper_move", 1024 * 2, NULL, 9, NULL);
}
