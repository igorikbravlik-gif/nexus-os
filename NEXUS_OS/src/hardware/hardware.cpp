#include "hardware/hardware.h"

namespace Hardware {
static Status st = {};

void begin() {
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  cfg.clear_display = true;
  cfg.output_power = false;  // не подаём 5V на разъём Grove
  cfg.internal_imu = true;
  cfg.internal_rtc = true;
  cfg.internal_spk = true;
  cfg.internal_mic = false;  // микрофон пока не нужен
  cfg.led_brightness = 0;
  M5.begin(cfg);

  // Сразу гасим подсветку: экран включится только на заставке
  M5.Display.setBrightness(0);
  M5.Display.fillScreen(0);

  st.boardOk = (M5.getBoard() == m5::board_t::board_M5StickCPlus2);
  st.imuOk = M5.Imu.isEnabled();
  st.rtcOk = M5.Rtc.isEnabled();
  st.spkOk = M5.Speaker.isEnabled();
  st.psramOk = psramFound();

  Serial.printf("[NEXUS] board=%d (PLUS2=%s) imu=%d rtc=%d spk=%d psram=%d\n", (int)M5.getBoard(),
                st.boardOk ? "yes" : "NO", st.imuOk, st.rtcOk, st.spkOk, st.psramOk);
}

const Status& status() { return st; }

int batteryPercent() {
  int v = M5.Power.getBatteryLevel();
  return (v < 0 || v > 100) ? -1 : v;
}

void setBacklight(uint8_t value) { M5.Display.setBrightness(value); }
void setVolume(uint8_t value) { M5.Speaker.setVolume(value); }
void beep(uint16_t freq, uint16_t ms) {
  if (st.spkOk) M5.Speaker.tone(freq, ms);
}
void powerOff() { M5.Power.powerOff(); }
}  // namespace Hardware
