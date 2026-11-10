#include "ui/display.h"
#include "ui/theme.h"

namespace UI {
static M5Canvas canvas(&M5.Display);
static bool ready = false;

bool begin() {
  M5.Display.setRotation(0);  // портрет 135x240
  M5.Display.fillScreen(TFT_BLACK);
  if (M5.Display.width() != Layout::W || M5.Display.height() != Layout::H) {
    Serial.printf("[NEXUS] unexpected display size %dx%d\n", M5.Display.width(), M5.Display.height());
  }
  canvas.setColorDepth(16);
  ready = (canvas.createSprite(Layout::W, Layout::H) != nullptr);
  if (!ready) Serial.println("[NEXUS] ERROR: sprite alloc failed");
  return ready;
}

M5Canvas& g() { return canvas; }

void clear(uint16_t color) {
  if (ready) canvas.fillScreen(color);
}

void present() {
  if (ready) canvas.pushSprite(0, 0);
}

float fitScale(const char* text, const lgfx::IFont* font, int maxW, float maxScale) {
  canvas.setFont(font);
  canvas.setTextSize(1.0f);
  int w = canvas.textWidth(text);
  if (w <= 0) return maxScale;
  float s = (float)maxW / (float)w;
  return s < maxScale ? s : maxScale;
}

void glowText(const char* text, int cx, int cy, const lgfx::IFont* font, float scale,
              uint16_t color, uint16_t halo, uint8_t alpha) {
  if (!ready) return;
  canvas.setFont(font);
  canvas.setTextSize(scale);
  canvas.setTextDatum(middle_center);

  // Внешнее свечение (8 направлений, смещение 2 px)
  static const int8_t d2[8][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}, {-2, -2}, {2, -2}, {-2, 2}, {2, 2}};
  canvas.setTextColor(Color::blend(halo, Color::Black, (uint8_t)(alpha * 0.18f)));
  for (auto& d : d2) canvas.drawString(text, cx + d[0], cy + d[1]);

  // Внутреннее свечение (4 направления, смещение 1 px)
  static const int8_t d1[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  canvas.setTextColor(Color::blend(halo, Color::Black, (uint8_t)(alpha * 0.40f)));
  for (auto& d : d1) canvas.drawString(text, cx + d[0], cy + d[1]);

  // Основной текст
  canvas.setTextColor(Color::blend(color, Color::Black, alpha));
  canvas.drawString(text, cx, cy);
}

void panel(int x, int y, int w, int h, uint16_t border, uint16_t fill) {
  if (!ready) return;
  canvas.fillRoundRect(x, y, w, h, 4, fill);
  canvas.drawRoundRect(x, y, w, h, 4, border);
}

void header(const char* title) {
  if (!ready) return;
  canvas.fillRect(0, 0, Layout::W, Layout::HEADER_H, Color::Panel);
  canvas.drawFastHLine(0, Layout::HEADER_H - 1, Layout::W, Theme::accent());
  canvas.setFont(&fonts::Font2);
  canvas.setTextSize(1);
  canvas.setTextDatum(middle_left);
  canvas.setTextColor(Theme::accent());
  canvas.drawString(title, Layout::MARGIN, Layout::HEADER_H / 2 - 1);
}

void hintBar(const char* a, const char* b, const char* c) {
  if (!ready) return;
  const int y = Layout::H - Layout::HINT_H;
  const int cy = y + Layout::HINT_H / 2 + 1;
  canvas.fillRect(0, y, Layout::W, Layout::HINT_H, Color::Panel);
  canvas.drawFastHLine(0, y, Layout::W, Color::DimGray);
  canvas.setFont(&fonts::Font0);
  canvas.setTextSize(1);
  canvas.setTextColor(Color::Gray);
  char buf[16];
  canvas.setTextDatum(middle_left);
  snprintf(buf, sizeof(buf), "A:%s", a);
  canvas.drawString(buf, 3, cy);
  canvas.setTextDatum(middle_center);
  snprintf(buf, sizeof(buf), "B:%s", b);
  canvas.drawString(buf, Layout::W / 2, cy);
  canvas.setTextDatum(middle_right);
  snprintf(buf, sizeof(buf), "C:%s", c);
  canvas.drawString(buf, Layout::W - 3, cy);
}
}  // namespace UI
