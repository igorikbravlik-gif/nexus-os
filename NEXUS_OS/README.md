# NEXUS OS (этапы 1-3)
Автор: Antoninapers. Цель: M5StickC PLUS2 (ESP32-PICO-V3-02, 8 MB flash).

## Сборка (Windows)
1. Установите VS Code и расширение PlatformIO IDE.
2. Драйвер USB: CH9102 (если порт COM не появляется).
3. File > Open Folder > папка NEXUS_OS. Дождитесь загрузки библиотек.
4. Сборка: значок галочки внизу (или `pio run`).
5. Готовый файл: `.pio\build\m5stickc_plus2\firmware.bin`

## Загрузка
Подключите устройство USB-C, нажмите стрелку (Upload) или `pio run -t upload`.

## Бэкап заводской прошивки (СДЕЛАЙТЕ ДО ПЕРВОЙ ЗАГРУЗКИ)
    python -m esptool --port COM3 --baud 921600 read_flash 0x0 0x800000 factory_backup.bin
Восстановление: `python -m esptool --port COM3 --baud 921600 write_flash 0x0 factory_backup.bin`
Или через M5Burner (заводская прошивка StickC PLUS2).

## Кнопки
A (GPIO37): click = выбор, long = доп. действия
B (GPIO39): click = назад, long = Home
C (GPIO35, кнопка питания): click = далее, double = доп. действие.
Долгое нажатие C зарезервировано за питанием и не используется как системное меню.

## Проверка этапа 3
- Заставка Antoninapers (fade in ~1 c, удержание, fade out) ~3 c.
- Затем тестовый экран: нажимайте кнопки, события должны появляться в рамке.
- Строка board должна быть "StickC PLUS2", imu/rtc = ok.
- Serial Monitor (115200): строка [NEXUS] board=...

## Готовый esp32fw.bin для M5Burner (без VS Code)
Файл esp32fw.bin - полный образ флеша (прошивается с адреса 0x0).
Получить его без установки программ:
1. Создайте репозиторий на github.com и загрузите в него СОДЕРЖИМОЕ папки NEXUS_OS
   (вместе со скрытой папкой .github).
2. Вкладка Actions -> Build NEXUS OS -> Run workflow.
3. Когда сборка станет зелёной, откройте её и скачайте артефакт esp32fw (zip) -> внутри esp32fw.bin.
4. M5Burner: добавьте локальный файл как свою прошивку (раздел пользовательских/custom прошивок),
   выберите порт COM и нажмите Burn.
Альтернатива без M5Burner: https://espressif.github.io/esptool-js/ , адрес 0x0, файл esp32fw.bin.
(С VS Code файл появится в корне проекта после обычной сборки.)
