#pragma once
#include <Arduino.h>

#ifndef NEXUS_VERSION
#define NEXUS_VERSION "0.0.0"
#endif
#define NEXUS_NAME "NEXUS OS"
#define NEXUS_AUTHOR "Antoninapers"
#define NEXUS_NICK_MAX 24

struct Settings {
  char nick[NEXUS_NICK_MAX];
  uint8_t brightness;      // 16..255
  bool soundOn;
  uint8_t volume;          // 0..255
  uint8_t accent;          // 0..3
  bool clock24;
  uint16_t screenTimeout;  // секунды, 0 = выключено
};

enum class Beep : uint8_t { Click, Back, Select, Error };

namespace System {
void begin();
Settings& settings();
void saveSettings();
void resetSettings();
void applySettings();            // яркость, громкость, акцент
void beep(Beep kind);            // учитывает настройки звука
uint32_t uptimeSec();
const char* resetReasonText();
}  // namespace System
