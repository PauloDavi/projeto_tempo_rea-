#include <stdint.h>
#include <string.h>

#include <string>

#include "fontx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ili9340.h"

typedef enum {
  BACK_BUTTON_ACTIVE,
  NEXT_BUTTON_ACTIVE,
  ACTION_ACTIVE,
  ACTION_SELECTED
} display_state_t;

typedef enum {
  SPLASH_SCREEN,
  START_SCREEN,
  ANGLE_SCREEN,
  DURATION_SCREEN,
  VELOCITY_SCREEN,
  SUMMARY_SCREEN,
  WARNING_SCREEN,
  COUNTDOWN_SCREEN,
  SECTION_SCREEN
} current_display_t;

struct display_control_t {
  display_state_t current_state = ACTION_ACTIVE;
  current_display_t current_display = SPLASH_SCREEN;
  uint8_t angle = 0;
  uint8_t duration = 0;
  uint8_t velocity = 1;
  TFT_t display;
  FontxFile fontx[2];
};

void start_screen_action_button_click(display_control_t *display_control, uint16_t color);

void angle_screen_action_clockwise(display_control_t *display_control, uint16_t color);
void angle_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color);
void angle_screen_action_button_click(display_control_t *display_control, uint16_t color);

void duration_screen_action_clockwise(display_control_t *display_control, uint16_t color);
void duration_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color);
void duration_screen_action_button_click(display_control_t *display_control, uint16_t color);

void velocity_screen_action_clockwise(display_control_t *display_control, uint16_t color);
void velocity_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color);
void velocity_screen_action_button_click(display_control_t *display_control, uint16_t color);

void summary_screen_action_clockwise(display_control_t *display_control, uint16_t color);
void summary_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color);
void summary_screen_action_button_click(display_control_t *display_control, uint16_t color);

void warning_screen_action_clockwise(display_control_t *display_control, uint16_t color);
void warning_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color);
void warning_screen_action_button_click(display_control_t *display_control, uint16_t color);

void warning_screen_action_clockwise(display_control_t *display_control, uint16_t color);
void warning_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color);
void warning_screen_action_button_click(display_control_t *display_control, uint16_t color);

void section_screen_action_button_click(display_control_t *display_control, uint16_t color);

// utils
std::string pad_left(std::string const &str, size_t s);

void initDisplay(display_control_t *display_control, uint16_t color);
void lcdDrawTitle(display_control_t *display_control, char *ascii, uint16_t color);
void lcdDrawButton(display_control_t *display_control, uint16_t centerX, uint16_t centerY, uint16_t px, uint16_t py, const char *ascii, uint16_t color, bool isSelected);

void lcdDrawSplashScreen(display_control_t *display_control, uint16_t color);
void lcdDrawStartScreen(display_control_t *display_control, uint16_t color);
void lcdDrawAngleScreen(display_control_t *display_control, uint16_t color);
void lcdDrawDurationScreen(display_control_t *display_control, uint16_t color);
void lcdDrawVelocityScreen(display_control_t *display_control, uint16_t color);
void lcdDrawSummaryScreen(display_control_t *display_control, uint16_t color);
void lcdDrawWarningScreen(display_control_t *display_control, uint16_t color);
void lcdDrawCountdownScreen(display_control_t *display_control, uint16_t color);
void lcdDrawSectionScreen(display_control_t *display_control, uint16_t color);