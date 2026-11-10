#include "system/system.h"
#include <Preferences.h>
#include <esp_system.h>
#include "hardware/hardware.h"
#include "ui/theme.h"

namespace {
constexpr uint8_t SCHEMA = 1;
Preferences prefs;
Settings cfg;

void setDefaults() {
  memset(&cfg, 0, sizeof(cfg));
  strncpy(cfg.nick, NEXUS_AUTHOR, NEXUS_NICK_MAX - 1);
  cfg.brightness = 130;
  cfg.soundOn = true;
  cfg.volume = 120;
  cfg.accent = 0;
  cfg.clock24 = true;
  cfg.screenTimeout = 0;
}

// Защита от повреждённых данных в NVS
void sanitize() {
  cfg.nick[NEXUS_NICK_MAX - 1] = 0;
  for (size_t i = 0; cfg.nick[i]; i++) {
    if (cfg.nick[i] < 32 || cfg.nick[i] > 126) cfg.nick[i] = '?';
  }
  if (cfg.nick[0] == 0) strncpy(cfg.nick, NEXUS_AUTHOR, NEXUS_NICK_MAX - 1);
  if (cfg.brightness < 16) cfg.brightness = 16;
  if (cfg.accent >= Theme::ACCENT_COUNT) cfg.accent = 0;
  if (cfg.screenTimeout > 3600) cfg.screenTimeout = 0;
}
}  // namespace

namespace System {
void begin() {
  setDefaults();
  if (prefs.begin("nexus", false)) {
    if (prefs.getUChar("schema", 0) == SCHEMA) {
      prefs.getString("nick", cfg.nick, NEXUS_NICK_MAX);
      cfg.brightness = prefs.getUChar("bright", cfg.brightness);
      cfg.soundOn = prefs.getBool("sound", cfg.soundOn);
      cfg.volume = prefs.getUChar("vol", cfg.volume);
      cfg.accent = prefs.getUChar("accent", cfg.accent);
      cfg.clock24 = prefs.getBool("clk24", cfg.clock24);
      cfg.screenTimeout = prefs.getUShort("tmo", cfg.screenTimeout);
    } else {
      prefs.putUChar("schema", SCHEMA);  // первый запуск
    }
    prefs.end();
  } else {
    Serial.println("[NEXUS] NVS open failed, using defaults");
  }
  sanitize();
  Theme::setAccent(cfg.accent);
  Hardware::setVolume(cfg.volume);
}

Settings& settings() { return cfg; }

void saveSettings() {
  sanitize();
  if (!prefs.begin("nexus", false)) return;
  prefs.putUChar("schema", SCHEMA);
  prefs.putString("nick", cfg.nick);
  prefs.putUChar("bright", cfg.brightness);
  prefs.putBool("sound", cfg.soundOn);
  prefs.putUChar("vol", cfg.volume);
  prefs.putUChar("accent", cfg.accent);
  prefs.putBool("clk24", cfg.clock24);
  prefs.putUShort("tmo", cfg.screenTimeout);
  prefs.end();
}

void resetSettings() {
  if (prefs.begin("nexus", false)) {
    prefs.clear();
    prefs.end();
  }
  setDefaults();
  saveSettings();
  applySettings();
}

void applySettings() {
  Theme::setAccent(cfg.accent);
  Hardware::setBacklight(cfg.brightness);
  Hardware::setVolume(cfg.volume);
}

void beep(Beep kind) {
  if (!cfg.soundOn) return;
  switch (kind) {
    case Beep::Click:  Hardware::beep(2400, 15); break;
    case Beep::Back:   Hardware::beep(1800, 20); break;
    case Beep::Select: Hardware::beep(3000, 30); break;
    case Beep::Error:  Hardware::beep(500, 120); break;
  }
}

uint32_t uptimeSec() { return millis() / 1000; }

const char* resetReasonText() {
  switch (esp_reset_reason()) {
    case ESP_RST_POWERON:   return "Power on";
    case ESP_RST_SW:        return "Software";
    case ESP_RST_PANIC:     return "Crash";
    case ESP_RST_INT_WDT:   return "Int WDT";
    case ESP_RST_TASK_WDT:  return "Task WDT";
    case ESP_RST_WDT:       return "WDT";
    case ESP_RST_DEEPSLEEP: return "Deep sleep";
    case ESP_RST_BROWNOUT:  return "Brownout";
    case ESP_RST_EXT:       return "External";
    default:                return "Unknown";
  }
}
}  // namespace System
