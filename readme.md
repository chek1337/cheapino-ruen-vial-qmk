> [!CAUTION]
> 🔴 **АЛЬФА-ВЕРСИЯ.** Прошивка не тестировалась на реальном устройстве. Используйте на свой страх и риск.

---

# Cheapino v2 — RuEn порт

Прошивка для клавиатуры **Cheapino v2** с поддержкой символов, не зависящих от активной раскладки (RU/EN).

---

## Основано на

- 📖 [Руководство RuEn от Ergohaven](https://github.com/ergohaven/vial-qmk/blob/ergohaven/keyboards/ergohaven/docs/ruen.md) — описание принципа работы и реализации
- 🔧 [Репозиторий порта RuEn на silakka54](https://github.com/getlikeman/silakka54-with-ruen-port) — опирался на него в качестве референса 
---

## Что было сделано

### Шаг 1 — Fork upstream vial-qmk

За основу взят fork upstream [vial-kb/vial-qmk](https://github.com/vial-kb/vial-qmk).

### Шаг 2 — Копирование файлов

По гайду из [tompi/cheapino#153 (comment)](https://github.com/tompi/cheapino/issues/153#issuecomment-3700628122) скопированы файлы из двух веток:

1. **Папка `keyboards/cheapino`** — из ветки [@schuay](https://github.com/schuay) с актуальными изменениями QMK:
   [`schuay/qmk_firmware:cheapino`](https://github.com/schuay/qmk_firmware/tree/cheapino)

2. **Папка `keyboards/cheapino/keymaps/vial`** — из ветки [@tompi](https://github.com/tompi) с vial-раскладкой для cheapino v2:
   [`tompi/vial-qmk:cheapinov2`](https://github.com/tompi/vial-qmk/tree/cheapinov2)

### Шаг 3 — Точечные правки для успешной компиляции

Без этих изменений `make cheapino:vial` завершается с ошибкой:

1. **`keyboards/cheapino/rules.mk`** — добавить строку:
   ```makefile
   ENCODER_ENABLE = yes
   ```
   *(необходимо для компиляции, без этого сборка падает и в обычном QMK)*

2. **`keyboards/cheapino/keymaps/vial/rules.mk`** — добавить строку:
   ```makefile
   VIA_ENABLE = yes
   ```
   *(без этого компиляция падает на стадии keymap introspection)*

3. **`keyboards/cheapino/matrix.c`** — исправить вызов `debounce_init` в функции `matrix_init_custom`:
   ```c
   debounce_init(MATRIX_ROWS);
   ```

   В актуальном vial-qmk функция `debounce_init` требует аргумент `num_rows`, тогда как в исходнике cheapino она вызывается без аргументов, что приводит к ошибке:
   ```
   keyboards/cheapino/matrix.c:119:5: error: too few arguments to function 'debounce_init'
     119 |     debounce_init();
   ```
   Альтернативный вариант — откатить коммит [qmk/qmk_firmware@2d59087](https://github.com/tompi/cheapino/issues/155#issuecomment-3702115808), но в данном порте сделана прямая правка файла.

   > Подробности: [tompi/cheapino#155 (comment)](https://github.com/tompi/cheapino/issues/155#issuecomment-3700605329)

---
### Шаг 4 — Порт RuEn мода и создание раскладки

Перенесён базовый функционал RuEn мода по [руководству Ergohaven](https://github.com/ergohaven/vial-qmk/blob/ergohaven/keyboards/ergohaven/docs/ruen.md) и [репозиторию silakka54-with-ruen-port](https://github.com/getlikeman/silakka54-with-ruen-port).

Создана раскладка для Vial по пути:
```
keyboards/cheapino/keymaps/ruen/vial.json
```


## Установка и сборка прошивки

### 1. Клонировать репозиторий

```bash
git clone git@github.com:chek1337/cheapino-ruen-vial-qmk.git
cd cheapino-ruen-vial-qmk
git checkout cheapinov2_ruen
```

### 2. Инициализировать субмодули

```bash
make git-submodule
```

### 3. Скомпилировать прошивку

```bash
qmk compile -kb cheapino -km ruen
```

