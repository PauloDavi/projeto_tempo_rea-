#include "display_control.h"

void start_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  display_control->current_state = BACK_BUTTON_ACTIVE;
  display_control->current_display = ANGLE_SCREEN;
  lcdDrawAngleScreen(display_control, color);
}

void angle_screen_action_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = ACTION_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    case ACTION_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case ACTION_SELECTED:
      if (display_control->angle < 110) {
        display_control->angle += 5;
        lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, false);
      }
  }
}

void angle_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, true);
      display_control->current_state = ACTION_ACTIVE;
      break;
    case ACTION_ACTIVE:
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    case ACTION_SELECTED:
      if (display_control->angle > 0) {
        display_control->angle -= 5;
        lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, false);
      }
  }
}

void angle_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      display_control->current_state = ACTION_ACTIVE;
      display_control->current_display = START_SCREEN;
      lcdDrawStartScreen(display_control, color);
      break;
    case NEXT_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = DURATION_SCREEN;
      lcdDrawDurationScreen(display_control, color);
      break;
    case ACTION_ACTIVE:
      display_control->current_state = ACTION_SELECTED;
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, false);
      break;
    case ACTION_SELECTED:
      display_control->current_state = ACTION_ACTIVE;
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, true);
  }
}

void duration_screen_action_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = ACTION_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    case ACTION_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case ACTION_SELECTED:
      if (display_control->duration < 30) {
        display_control->duration++;
        lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, false);
      }
  }
}

void duration_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, true);
      display_control->current_state = ACTION_ACTIVE;
      break;
    case ACTION_ACTIVE:
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    case ACTION_SELECTED:
      if (display_control->duration > 0) {
        display_control->duration--;
        lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, false);
      }
  }
}

void duration_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = ANGLE_SCREEN;
      lcdDrawAngleScreen(display_control, color);
      break;
    case NEXT_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = VELOCITY_SCREEN;
      lcdDrawVelocityScreen(display_control, color);
      break;
    case ACTION_ACTIVE:
      display_control->current_state = ACTION_SELECTED;
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, false);
      break;
    case ACTION_SELECTED:
      display_control->current_state = ACTION_ACTIVE;
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, true);
  }
}

void velocity_screen_action_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = ACTION_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    case ACTION_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case ACTION_SELECTED:
      if (display_control->velocity < 3) {
        display_control->velocity++;
        lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, false);
      }
  }
}

void velocity_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, true);
      display_control->current_state = ACTION_ACTIVE;
      break;
    case ACTION_ACTIVE:
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    case ACTION_SELECTED:
      if (display_control->velocity > 1) {
        display_control->velocity--;
        lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, false);
      }
  }
}

void velocity_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = DURATION_SCREEN;
      lcdDrawDurationScreen(display_control, color);
      break;
    case NEXT_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = SUMMARY_SCREEN;
      lcdDrawSummaryScreen(display_control, color);
      break;
    case ACTION_ACTIVE:
      display_control->current_state = ACTION_SELECTED;
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, false);
      break;
    case ACTION_SELECTED:
      display_control->current_state = ACTION_ACTIVE;
      lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, true);
  }
}

void summary_screen_action_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    default:
      break;
  }
}

void summary_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    default:
      break;
  }
}

void summary_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = VELOCITY_SCREEN;
      lcdDrawVelocityScreen(display_control, color);
      break;
    case NEXT_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = WARNING_SCREEN;
      lcdDrawWarningScreen(display_control, color);
      break;
    default:
      break;
  }
}

void warning_screen_action_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Iniciar", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Cancelar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Iniciar", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Cancelar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    default:
      break;
  }
}

void warning_screen_action_anti_clockwise(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Iniciar", color, true);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Cancelar", color, false);
      display_control->current_state = NEXT_BUTTON_ACTIVE;
      break;
    case NEXT_BUTTON_ACTIVE:
      lcdDrawButton(display_control, 240, 200, 12, 4, "Iniciar", color, false);
      lcdDrawButton(display_control, 80, 200, 12, 4, "Cancelar", color, true);
      display_control->current_state = BACK_BUTTON_ACTIVE;
      break;
    default:
      break;
  }
}

void warning_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  switch (display_control->current_state) {
    case BACK_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = START_SCREEN;
      display_control->angle = 0;
      display_control->duration = 0;
      display_control->velocity = 1;
      lcdDrawStartScreen(display_control, color);
      break;
    case NEXT_BUTTON_ACTIVE:
      display_control->current_state = BACK_BUTTON_ACTIVE;
      display_control->current_display = COUNTDOWN_SCREEN;
      lcdDrawCountdownScreen(display_control, color);
      break;
    default:
      break;
  }
}

void section_screen_action_button_click(display_control_t *display_control, uint16_t color) {
  display_control->current_state = BACK_BUTTON_ACTIVE;
  display_control->current_display = START_SCREEN;
  display_control->angle = 0;
  display_control->duration = 0;
  display_control->velocity = 1;
  lcdDrawStartScreen(display_control, color);
}

// utils
std::string pad_left(std::string const &str, size_t s) {
  if (str.size() < s)
    return std::string(s - str.size(), ' ') + str;
  else
    return str;
}

void lcdDrawTitle(display_control_t *display_control, char *ascii, uint16_t color) {
  int paddingY = 32 + 4 * 2;
  int length = strlen(ascii);
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - length * 16 / 2, paddingY / 2 + 16, (uint8_t *)ascii, color);
  lcdDrawFillRect(&(display_control->display), 0, paddingY, display_control->display._width, paddingY + 2, color);
}

void lcdDrawButton(
    display_control_t *display_control,
    uint16_t centerX,
    uint16_t centerY,
    uint16_t px,
    uint16_t py,
    const char *ascii,
    uint16_t color,
    bool isSelected) {
  uint16_t length = strlen(ascii);
  uint16_t width = (length * 16) + (2 * px);
  uint16_t height = 32 + (2 * py);

  uint16_t textColor = isSelected ? WHITE : color;
  uint16_t bgColor = isSelected ? color : WHITE;

  lcdDrawFillRect(&(display_control->display), centerX - (width / 2), centerY - (height / 2), centerX + (width / 2), centerY + (height / 2), bgColor);
  lcdDrawRect(&(display_control->display), centerX - (width / 2), centerY - (height / 2), centerX + (width / 2), centerY + (height / 2), color);
  lcdDrawString(&(display_control->display), display_control->fontx, centerX - length * 16 / 2, centerY + 16, (uint8_t *)ascii, textColor);
}

void initDisplay(display_control_t *display_control, uint16_t color) {
  InitFontx(display_control->fontx, "/spiffs/LATIN32B.FNT", "");

  spi_master_init(&(display_control->display), 27, 26, 13, 14, 12, 25, -1, -1, -1);

  lcdInit(&(display_control->display), 0x9341, 320, 240, 0, 0);

  lcdDrawSplashScreen(display_control, color);

  vTaskDelay(5000 / portTICK_PERIOD_MS);

  display_control->current_display = START_SCREEN;
  lcdDrawStartScreen(display_control, color);
}

void lcdDrawSplashScreen(display_control_t *display_control, uint16_t color) {
  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 17 * 16 / 2, display_control->display._height / 2 + 16, (uint8_t *)"CPM Fisiotech", color);
}

void lcdDrawStartScreen(display_control_t *display_control, uint16_t color) {
  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Controle da perna", color);
  lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, "Iniciar", color, true);
}

void lcdDrawAngleScreen(display_control_t *display_control, uint16_t color) {
  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Definir Amplitude", color);
  lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->angle), 3) + " Graus").c_str(), color, false);
  lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
  lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
}

void lcdDrawDurationScreen(display_control_t *display_control, uint16_t color) {
  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Definir Duracao", color);
  lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, (pad_left(std::__cxx11::to_string(display_control->duration), 2) + " min").c_str(), color, false);
  lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
  lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
}

void lcdDrawVelocityScreen(display_control_t *display_control, uint16_t color) {
  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Definir Velocidade", color);
  lcdDrawButton(display_control, display_control->display._width / 2, display_control->display._height / 2, 12, 4, ("Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str(), color, false);
  lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
  lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
}

void lcdDrawSummaryScreen(display_control_t *display_control, uint16_t color) {
  char text[40];

  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Resumo Sessao", color);
  strcpy(text, ("- " + std::__cxx11::to_string(display_control->angle) + " Graus").c_str());
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2 - 32, (uint8_t *)&text, color);
  strcpy(text, ("- " + std::__cxx11::to_string(display_control->duration) + " min").c_str());
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2, (uint8_t *)&text, color);
  strcpy(text, ("- Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str());
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2 + 32, (uint8_t *)&text, color);
  lcdDrawButton(display_control, 240, 200, 12, 4, "Proximo", color, false);
  lcdDrawButton(display_control, 80, 200, 12, 4, "Voltar", color, true);
}

void lcdDrawWarningScreen(display_control_t *display_control, uint16_t color) {
  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Inicio de Sessao", color);
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2 - 16, (uint8_t *)"Tem certeza que", color);
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2 + 16, (uint8_t *)"deseja iniciar?", color);
  lcdDrawButton(display_control, 240, 200, 12, 4, "Iniciar", color, false);
  lcdDrawButton(display_control, 80, 200, 12, 4, "Cancelar", color, true);
}

void lcdDrawSectionScreen(display_control_t *display_control, uint16_t color) {
  char text[40];

  lcdFillScreen(&(display_control->display), WHITE);
  lcdDrawTitle(display_control, "Sessao em andamento", color);
  strcpy(text, ("- " + std::__cxx11::to_string(display_control->angle) + " Graus").c_str());
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2 - 32, (uint8_t *)&text, color);
  strcpy(text, ("- " + std::__cxx11::to_string(display_control->duration) + " min").c_str());
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2, (uint8_t *)&text, color);
  strcpy(text, ("- Velocidade " + std::__cxx11::to_string(display_control->velocity)).c_str());
  lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 120, display_control->display._height / 2 + 32, (uint8_t *)&text, color);
  lcdDrawButton(display_control, display_control->display._width / 2, 200, 12, 4, "Cancelar", color, true);
}

void lcdDrawCountdownScreen(display_control_t *display_control, uint16_t color) {
  char text[1];

  for (uint8_t i = 5; i > 0; i--) {
    lcdFillScreen(&(display_control->display), WHITE);
    strcpy(text, std::__cxx11::to_string(i).c_str());
    lcdDrawString(&(display_control->display), display_control->fontx, display_control->display._width / 2 - 8, display_control->display._height / 2 + 16, (uint8_t *)&text, color);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  lcdDrawSectionScreen(display_control, color);
  display_control->current_display = SECTION_SCREEN;
}