#include "ui/input.h"
#include "hardware/hardware.h"

namespace Input {
namespace {
constexpr uint32_t DEBOUNCE_MS = 25;
constexpr uint32_t LONG_MS = 700;
constexpr uint32_t DOUBLE_MS = 280;  // окно двойного нажатия (только для C)

struct Button {
  uint8_t pin;
  Btn id;
  bool allowDouble;
  bool raw;
  bool stable;
  bool longFired;
  bool clickPending;
  bool second;
  uint32_t rawChangedAt;
  uint32_t pressedAt;
  uint32_t releasedAt;
};

Button btns[3] = {
    {Pin::BTN_A, Btn::A, false, false, false, false, false, false, 0, 0, 0},
    {Pin::BTN_B, Btn::B, false, false, false, false, false, false, 0, 0, 0},
    {Pin::BTN_C, Btn::C, true, false, false, false, false, false, 0, 0, 0},
};

constexpr uint8_t QN = 8;
InputEvent q[QN];
uint8_t qh = 0, qt = 0;

void push(Btn b, EvType t) {
  uint8_t n = (qt + 1) % QN;
  if (n == qh) return;  // очередь полна - событие отбрасывается
  q[qt] = {b, t};
  qt = n;
}
}  // namespace

void begin() {
  uint32_t now = millis();
  for (auto& b : btns) {
    pinMode(b.pin, INPUT);  // GPIO35/37/39 - только вход, подтяжка внешняя
    b.raw = b.stable = (digitalRead(b.pin) == LOW);
    b.rawChangedAt = now;
    b.longFired = b.stable;  // кнопка зажата при старте (включение) - не даём ложный Click
    b.clickPending = false;
    b.second = false;
  }
}

void update() {
  uint32_t now = millis();
  for (auto& b : btns) {
    bool r = (digitalRead(b.pin) == LOW);  // нажата = LOW
    if (r != b.raw) {
      b.raw = r;
      b.rawChangedAt = now;
    }
    if (b.raw != b.stable && (now - b.rawChangedAt) >= DEBOUNCE_MS) {
      b.stable = b.raw;
      if (b.stable) {  // нажатие
        b.pressedAt = now;
        b.longFired = false;
        b.second = b.allowDouble && b.clickPending && (now - b.releasedAt) <= DOUBLE_MS;
      } else if (!b.longFired) {  // отпускание без долгого удержания
        if (b.second) {
          push(b.id, EvType::Double);
          b.clickPending = false;
          b.second = false;
        } else if (b.allowDouble) {
          b.clickPending = true;  // ждём возможное второе нажатие
          b.releasedAt = now;
        } else {
          push(b.id, EvType::Click);
        }
      }
    }
    if (b.stable && !b.longFired && (now - b.pressedAt) >= LONG_MS) {
      b.longFired = true;
      b.clickPending = false;
      b.second = false;
      push(b.id, EvType::Long);
    }
    if (b.clickPending && !b.stable && (now - b.releasedAt) > DOUBLE_MS) {
      b.clickPending = false;
      push(b.id, EvType::Click);
    }
  }
}

bool poll(InputEvent& ev) {
  if (qh == qt) return false;
  ev = q[qh];
  qh = (qh + 1) % QN;
  return true;
}

void flush() { qh = qt; }

const char* name(const InputEvent& ev) {
  static char buf[16];
  const char* b = ev.btn == Btn::A ? "A" : ev.btn == Btn::B ? "B" : "C";
  const char* t = ev.type == EvType::Click ? "click" : ev.type == EvType::Long ? "long" : "double";
  snprintf(buf, sizeof(buf), "%s %s", b, t);
  return buf;
}
}  // namespace Input
