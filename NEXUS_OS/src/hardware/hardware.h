#pragma once
#include <M5Unified.h>

// Распиновка M5StickC PLUS2 (по документации M5Stack)
namespace Pin {
constexpr uint8_t BTN_A = 37;  // большая кнопка на лицевой стороне
constexpr uint8_t BTN_B = 39;  // боковая кнопка
constexpr uint8_t BTN_C = 35;  // кнопка питания (читается как обычный вход)
constexpr uint8_t IR_TX = 19;
constexpr uint8_t BUZZER = 2;
constexpr uint8_t PWR_HOLD = 4;  // удержание питания, управляется M5Unified
}  // namespace Pin

namespace Hardware {
struct Status {
  bool boardOk;  // M5Unified определил именно StickC PLUS2
  bool imuOk;
  bool rtcOk;
  bool spkOk;
  bool psramOk;
};

void begin();
const Status& status();
int batteryPercent();  // -1 если недоступно
void setBacklight(uint8_t value);
void setVolume(uint8_t value);
void beep(uint16_t freq, uint16_t ms);
void powerOff();
}  // namespace Hardware
