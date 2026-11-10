// Заставка Antoninapers: Fade In -> удержание -> Fade Out, всего ~3 секунды
#include "system/boot.h"
#include "hardware/hardware.h"
#include "system/system.h"
#include "ui/display.h"
#include "ui/theme.h"

namespace Boot {
namespace {
constexpr uint32_t FADE_IN_MS = 1000;
constexpr uint32_t HOLD_MS = 1200;
constexpr uint32_t FADE_OUT_MS = 800;
constexpr uint32_t TOTAL_MS = FADE_IN_MS + HOLD_MS + FADE_OUT_MS;  // 3000
constexpr uint32_t FRAME_MS = 33;

uint32_t t0 = 0, lastFrame = 0;

float smooth(float x) {
  if (x < 0) x = 0;
  if (x > 1) x = 1;
  return x * x * (3.0f - 2.0f * x);
}
}  // namespace

void begin() {
  UI::clear(Color::Black);
  UI::present();  // сначала чёрный кадр, и только потом подсветка - без артефактов
  Hardware::setBacklight(System::settings().brightness);
  t0 = millis();
  lastFrame = 0;
}

bool update() {
  uint32_t now = millis();
  uint32_t t = now - t0;
  if (t >= TOTAL_MS) {
    UI::clear(Color::Black);
    UI::present();
    return true;
  }
  if (now - lastFrame < FRAME_MS) return false;
  lastFrame = now;

  float a;
  if (t < FADE_IN_MS) a = smooth((float)t / FADE_IN_MS);
  else if (t < FADE_IN_MS + HOLD_MS) a = 1.0f;
  else a = 1.0f - smooth((float)(t - FADE_IN_MS - HOLD_MS) / FADE_OUT_MS);

  const char* nick = System::settings().nick;
  const lgfx::IFont* font = &fonts::Orbitron_Light_24;
  float scale = UI::fitScale(nick, font, Layout::W - 14, 1.0f);

  UI::clear(Color::Black);
  UI::glowText(nick, Layout::W / 2, Layout::H / 2, font, scale, Color::Blue, Color::Violet,
               (uint8_t)(a * 255.0f));
  UI::present();
  return false;
}
}  // namespace Boot
