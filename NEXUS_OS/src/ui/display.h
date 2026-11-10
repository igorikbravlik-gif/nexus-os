// NEXUS OS - слой отрисовки: двойная буферизация (спрайт 135x240) и примитивы
#pragma once
#include <M5Unified.h>

namespace UI {
bool begin();
M5Canvas& g();  // кадровый буфер
void clear(uint16_t color = 0x0000);
void present();  // вывод буфера на экран

// Подбор масштаба шрифта, чтобы текст влез в maxW (результат <= maxScale)
float fitScale(const char* text, const lgfx::IFont* font, int maxW, float maxScale = 1.0f);

// Текст со свечением по центру точки (cx, cy); alpha 0..255 - общая яркость
void glowText(const char* text, int cx, int cy, const lgfx::IFont* font, float scale,
              uint16_t color, uint16_t halo, uint8_t alpha);

void panel(int x, int y, int w, int h, uint16_t border, uint16_t fill);
void header(const char* title);
void hintBar(const char* a, const char* b, const char* c);  // до 5 символов на подпись
}  // namespace UI
