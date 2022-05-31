#include "driver/mcpwm.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/mcpwm_ll.h"
#include "stepper/stepper.h"

static const char* TAG = "exmple";

void stepper_move(void* pvParameters) {
  Stepper stepper(
      GPIO_NUM_13,
      GPIO_NUM_14,
      GPIO_NUM_15,
      32,
      1.8,
      4.2);

  stepper.begin();

  stepper.move(0, 100000, 1000);

  for (;;) {
    vTaskDelay(portMAX_DELAY);
  }
}

extern "C" void app_main(void) {
  xTaskCreate(stepper_move,   /* Function that implements the task. */
              "stepper_move", /* Text name for the task. */
              1024 * 2,       /* Stack size in words, not bytes. */
              0,              /* Parameter passed into the task. */
              10,             /* Priority at which the task is created. */
              NULL);
}
