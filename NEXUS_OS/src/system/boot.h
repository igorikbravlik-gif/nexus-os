#pragma once
namespace Boot {
void begin();    // чёрный кадр, затем включение подсветки
bool update();   // неблокирующая анимация; true = заставка завершена
}
