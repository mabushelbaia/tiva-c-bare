# tiva-c-bare

Bare-metal firmware for the **TM4C123GH6PM** on the **EK-TM4C123GXL Launchpad**,
built from the command line with `arm-none-eabi-gcc` and flashed with
**lm4flash**. No vendor SDK: registers are accessed through the vendored CMSIS
device headers (struct style, e.g. `GPIOF->DATA`), so there is nothing external
to install or point the build at.

Right now it cycles the on-board RGB LED (PF1/PF2/PF3) through seven colors,
driven by a periodic Timer1A interrupt — the starting point for the micromouse
code.

## Layout

```
.
├── Makefile        build / flash / size / format / clean
├── linker.ld       TM4C123GH6PM memory map (256K flash, 32K SRAM)
├── device/         CMSIS register headers + startup/system
│   ├── startup_TM4C123.c   vector table + reset handler (weak named handlers)
│   ├── system_TM4C123.c    SystemInit / SystemCoreClock (clock + FPU setup)
│   └── cmsis_core/         ARM CMSIS core headers
├── docs/           pinout + board docs
├── external/       third-party code (mpaland/printf submodule + config)
├── src/            application source (main.c for now)
└── tools/          helper scripts (githooks/pre-commit)
```

## Register access

Registers are poked directly through the vendored CMSIS device headers (the
"Keil" struct style): `#include "TM4C123.h"`, then
`SYSCTL->RCGCGPIO |= (1U << 5);`, `GPIOF->DATA ^= (1U << 3);`, and so on. The
Cortex-M core intrinsics come with it (`NVIC_EnableIRQ()`, `__NOP()`,
`__disable_irq()`, ...). The `device/` headers come from Keil's `Keil.TM4C_DFP`
and ARM's `ARM.CMSIS` packs (see `LICENSE`).

Startup and clock setup are owned by the vendored `device/` files.
`device/startup_TM4C123.c` provides the vector table and a `Reset_Handler` that
copies `.data` to SRAM, zeroes `.bss`, runs `SystemInit()`, then calls `main()`.
Every vector is a weak alias to the default handler, so the app overrides one
just by defining a function of the same name (e.g. `TIMER1A_Handler` in
`src/main.c`). `device/system_TM4C123.c` provides `SystemInit` and
`SystemCoreClock`; call `SystemCoreClockUpdate()` to resync the latter if you
change the clock at runtime.

## Prerequisites

```sh
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib lm4flash   # CachyOS/Arch
```

Clone with submodules to get `external/printf` (mpaland's tiny printf, compiled
into the build and tuned by `external/printf_config.h`):

```sh
git clone --recurse-submodules <repo-url>
# or, in an existing clone:
git submodule update --init
```

## Build & flash

```sh
make          # -> build/bin/blinky.{axf,bin}  (objects in build/obj/)
make flash    # build + flash with lm4flash
make size     # binary size
make clean
```

## Formatting

```sh
make format        # clang-format src/ in place
make format-check  # fail if anything is unformatted
```

An optional git pre-commit hook lives in `tools/githooks/`; it blocks commits
that leave `src/` C sources unformatted. Enable it once per clone:

```sh
git config core.hooksPath tools/githooks
```

The vendored `external/printf` and the `device/` CMSIS files are never touched.
