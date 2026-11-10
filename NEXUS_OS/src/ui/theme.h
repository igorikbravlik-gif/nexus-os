// NEXUS OS - дизайн-система: цвета, размеры, акценты
#pragma once
#include <stdint.h>

namespace Layout {
constexpr int W = 135;
constexpr int H = 240;
constexpr int HEADER_H = 20;
constexpr int HINT_H = 16;
constexpr int MARGIN = 6;
}  // namespace Layout

namespace Color {
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return (uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}
constexpr uint16_t Black   = 0x0000;
constexpr uint16_t White   = 0xFFFF;
constexpr uint16_t Blue    = rgb(0, 160, 255);
constexpr uint16_t Violet  = rgb(150, 70, 255);
constexpr uint16_t Cyan    = rgb(0, 230, 210);
constexpr uint16_t Magenta = rgb(255, 50, 190);
constexpr uint16_t Gray    = rgb(140, 140, 160);
constexpr uint16_t DimGray = rgb(60, 60, 80);
constexpr uint16_t Panel   = rgb(10, 10, 22);
constexpr uint16_t Ok      = rgb(0, 230, 120);
constexpr uint16_t Warn    = rgb(255, 190, 0);
constexpr uint16_t Err     = rgb(255, 60, 80);

// Смешивание цветов RGB565: a=0 -> bg, a=255 -> fg
inline uint16_t blend(uint16_t fg, uint16_t bg, uint8_t a) {
  int fr = (fg >> 11) & 0x1F, fgc = (fg >> 5) & 0x3F, fb = fg & 0x1F;
  int br = (bg >> 11) & 0x1F, bgc = (bg >> 5) & 0x3F, bb = bg & 0x1F;
  int r = br + ((fr - br) * a) / 255;
  int g = bgc + ((fgc - bgc) * a) / 255;
  int b = bb + ((fb - bb) * a) / 255;
  return (uint16_t)((r << 11) | (g << 5) | b);
}
}  // namespace Color

namespace Theme {
constexpr uint8_t ACCENT_COUNT = 4;
inline uint8_t& accentIndex() {
  static uint8_t idx = 0;
  return idx;
}
inline void setAccent(uint8_t i) { accentIndex() = (i < ACCENT_COUNT) ? i : 0; }
inline uint16_t accent() {
  static const uint16_t t[ACCENT_COUNT] = {Color::Blue, Color::Violet, Color::Cyan, Color::Magenta};
  return t[accentIndex()];
}
// Второй (дополнительный) цвет: фиолетовый, а для фиолетового акцента - синий
inline uint16_t accent2() { return accentIndex() == 1 ? Color::Blue : Color::Violet; }
}  // namespace Theme
