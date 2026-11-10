// NEXUS OS by Antoninapers - для M5StickC PLUS2
// Этапы 1-3: аппаратная база, настройки, ввод, заставка.
// После заставки показывается ВРЕМЕННЫЙ экран проверки кнопок (до этапа 4: Home и Launcher).
#include <M5Unified.h>
#include "hardware/hardware.h"
#include "system/boot.h"
#include "system/system.h"
#include "ui/display.h"
#include "ui/input.h"
#include "ui/theme.h"

enum class State : uint8_t { Boot, InputTest };
static State state = State::Boot;

static char log3[3][16];
static uint32_t lastDraw = 0;

static void pushLog(const char* s) {
  strcpy(log3[2], log3[1]);
  strcpy(log3[1], log3[0]);
  strncpy(log3[0], s, 15);
  log3[0][15] = 0;
}

static void drawInputTest() {
  auto& g = UI::g();
  UI::clear(Color::Black);
  UI::header(NEXUS_NAME);

  g.setFont(&fonts::Font2);
  g.setTextSize(1);
  g.setTextDatum(top_left);
  g.setTextColor(Color::White);
  g.drawString("INPUT TEST", Layout::MARGIN, 30);
  g.setTextColor(Color::Gray);
  g.drawString("v" NEXUS_VERSION, Layout::MARGIN, 48);

  char buf[24];
  uint32_t s = System::uptimeSec();
  snprintf(buf, sizeof(buf), "up %02lu:%02lu", (unsigned long)(s / 60), (unsigned long)(s % 60));
  g.drawString(buf, Layout::MARGIN, 66);

  UI::panel(Layout::MARGIN, 90, Layout::W - 2 * Layout::MARGIN, 62, Theme::accent(), Color::Panel);
  g.setTextColor(Theme::accent());
  g.drawString(log3[0][0] ? log3[0] : "press a button", Layout::MARGIN + 6, 96);
  g.setTextColor(Color::Gray);
  g.drawString(log3[1], Layout::MARGIN + 6, 114);
  g.setTextColor(Color::DimGray);
  g.drawString(log3[2], Layout::MARGIN + 6, 132);

  const auto& hw = Hardware::status();
  g.setFont(&fonts::Font0);
  g.setTextColor(hw.boardOk ? Color::Ok : Color::Err);
  g.drawString(hw.boardOk ? "board: StickC PLUS2" : "board: NOT PLUS2!", Layout::MARGIN, 162);
  g.setTextColor(hw.imuOk ? Color::Ok : Color::Err);
  g.drawString(hw.imuOk ? "imu: ok" : "imu: FAIL", Layout::MARGIN, 174);
  g.setTextColor(hw.rtcOk ? Color::Ok : Color::Err);
  g.drawString(hw.rtcOk ? "rtc: ok" : "rtc: FAIL", Layout::MARGIN, 186);
  g.setTextColor(hw.psramOk ? Color::Ok : Color::Warn);
  g.drawString(hw.psramOk ? "psram: ok" : "psram: none", Layout::MARGIN, 198);
  int bat = Hardware::batteryPercent();
  if (bat >= 0) snprintf(buf, sizeof(buf), "battery: %d%%", bat);
  else snprintf(buf, sizeof(buf), "battery: n/a");
  g.setTextColor(Color::Gray);
  g.drawString(buf, Layout::MARGIN, 210);

  UI::hintBar("Sel", "Back", "Next");
  UI::present();
}

void setup() {
  Hardware::begin();      // дисплей и периферия инициализируются до заставки
  System::begin();        // настройки из NVS
  UI::begin();
  Input::begin();
  Boot::begin();          // заставка Antoninapers
}

void loop() {
  M5.update();
  Input::update();

  switch (state) {
    case State::Boot:
      if (Boot::update()) {
        Input::flush();  // нажатия во время заставки не передаются дальше
        state = State::InputTest;
      }
      break;
    case State::InputTest: {
      InputEvent ev;
      while (Input::poll(ev)) {
        pushLog(Input::name(ev));
        System::beep(ev.type == EvType::Click ? Beep::Click : Beep::Select);
        lastDraw = 0;
      }
      if (millis() - lastDraw >= 100) {
        lastDraw = millis();
        drawInputTest();
      }
      break;
    }
  }
  delay(1);
}
