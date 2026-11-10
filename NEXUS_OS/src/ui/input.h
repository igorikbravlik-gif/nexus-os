// NEXUS OS - обработка трёх кнопок: debounce, click / long / double, очередь событий
#pragma once
#include <Arduino.h>

enum class Btn : uint8_t { A, B, C };
enum class EvType : uint8_t { Click, Long, Double };

struct InputEvent {
  Btn btn;
  EvType type;
};

namespace Input {
void begin();
void update();                 // вызывать каждый проход loop()
bool poll(InputEvent& ev);     // достать событие из очереди
void flush();                  // очистить очередь
const char* name(const InputEvent& ev);
}  // namespace Input
