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


## Режимы сборки

Поддерживаются **два режима**, переключаются флагом `VIAL_ENABLE` (и `VIA_ENABLE`) в `keyboards/cheapino/keymaps/ruen/rules.mk`.

| Режим | `VIAL_ENABLE` | `VIA_ENABLE` | `COMBO_ENABLE` | Что даёт |
|---|---|---|---|---|
| **Pure QMK** (по умолчанию) | `no` | `no` | `no` | Корректный per-key `HOLD_ON_OTHER_KEY_PRESS` для thumb-LT, корректный `CHORDAL_HOLD`, `PERMISSIVE_HOLD`. HRM не срабатывает жадно при кросс-руке. Combos и runtime-перенастройка раскладки **недоступны**. |
| **Vial** | `yes` | `yes` | `yes` | Полный Vial GUI: runtime remap, runtime combos, runtime tap-hold settings. Per-key callback'и из `keymap.c` игнорируются (их перехватывает `quantum/qmk_settings.c`); поведение tap-hold настраивается только через Vial GUI. |

Код в `config.h` и `keymap.c` обёрнут в `#ifndef QMK_SETTINGS` — наши `#define`'ы и override `get_hold_on_other_key_press` подключаются только в pure-QMK режиме, в Vial-режиме они автоматически отключаются.

### Когда какой режим использовать

- **Pure QMK** — основной рабочий режим. HRM и thumb-LT работают как задумано.
- **Vial** — если нужно временно поэкспериментировать с раскладкой через GUI без пересборки. Будь готов, что HOOKP/Chordal Hold нужно включать в Vial GUI вручную (toggles в настройках tap-hold), и HRM может срабатывать как мод при быстром кросс-hand наборе.

---

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

### 3. Выбрать режим сборки

Открыть `keyboards/cheapino/keymaps/ruen/rules.mk` и выставить флаги:

**Pure QMK (рекомендуется):**
```makefile
VIA_ENABLE = no
VIAL_ENABLE = no
COMBO_ENABLE = no
```

**Vial:**
```makefile
VIA_ENABLE = yes
VIAL_ENABLE = yes
COMBO_ENABLE = yes
```

### 4. Скомпилировать прошивку

```bash
qmk compile -kb cheapino -km ruen
```

UF2 будет создан в `.build/cheapino_ruen.uf2`.

### 5. Прошивка

Поставить плату в bootloader (зажать boot при подключении USB или нажать `QK_BOOT` с уже прошитой клавиатуры) и скопировать `.uf2` на появившийся том `RPI-RP2`.

---

## Сборка через Docker

Альтернатива установке QMK CLI и ARM-тулчейна локально — собирать прошивку в контейнере на базе официального образа [`qmkfm/qmk_cli`](https://hub.docker.com/r/qmkfm/qmk_cli).

В корне репозитория лежат:

- `Dockerfile` — минимальный образ поверх `qmkfm/qmk_cli`
- `docker.mk` — Makefile с командами `build`, `compile`, `clean`, `shell`
- `.dockerignore` — исключает `.build/`, `.git/` и артефакты из контекста сборки

### Требования

- Установленный Docker
- Инициализированные субмодули (см. шаг 2 выше): `make git-submodule`

### Команды

```bash
# 1. Собрать Docker-образ (один раз)
make -f docker.mk build

# 2. Скомпилировать прошивку (по умолчанию cheapino:ruen)
make -f docker.mk compile
```

UF2 окажется в `.build/cheapino_ruen.uf2` на хост-машине — рабочая директория монтируется в контейнер.

### Дополнительно

| Команда | Что делает |
|---|---|
| `make -f docker.mk clean` | Удалить `.build/` |
| `make -f docker.mk shell` | Открыть интерактивный bash в контейнере (для отладки) |

### Сборка другой раскладки

Переменные `KB` и `KM` можно переопределить:

```bash
make -f docker.mk compile KM=vial
make -f docker.mk compile KB=cheapino KM=default
```

