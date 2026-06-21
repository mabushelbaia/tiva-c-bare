# tiva-c-micromouse

Firmware for the **TM4C123GH6PM** on the **EK-TM4C123GXL Launchpad**, built from
the command line with `arm-none-eabi-gcc` against the **TivaWare SDK** and
flashed with **lm4flash**.

Right now it's a blinky (on-board green LED) - the starting point for the
micromouse code.

## Layout

```
.
├── Makefile        build / flash / size / format / clean
├── linker.ld       TM4C123GH6PM memory map (256K flash, 32K SRAM)
├── startup_gcc.c   vector table + reset handler (verbatim from TivaWare)
├── device/         CMSIS register headers (struct style; see below)
├── docs/           pinout
├── external/       third-party code (mpaland/printf submodule)
├── src/            application source (main.c for now)
└── tools/          helper scripts (githooks/pre-commit)
```

## Register access: two styles, your choice

Both register APIs are wired up; pick whichever fits per source file. Just don't
`#include` both register headers in the *same* `.c` - they cover the same
peripherals under different names.

- **TivaWare driverlib** (what `src/main.c` uses) - `#include` the `driverlib/`
  and `inc/` headers and call `GPIOPinWrite()`, `SysCtlClockSet()`, etc.
- **CMSIS struct style** (the "Keil" style) - `#include "TM4C123GH6PM.h"` and
  poke registers directly: `GPIOF->DATA ^= (1U << 3);`,
  `SYSCTL->RCGCGPIO |= (1U << 5);`. The Cortex-M core intrinsics come with it
  (`NVIC_EnableIRQ()`, `__NOP()`, `__disable_irq()`, ...).

Startup and the system clock are owned by TivaWare here (`startup_gcc.c` +
`SysCtlClockSet()`), so the CMSIS `startup_gcc.s` is intentionally *not*
included. `device/system_TM4C123.c` is built in only so CMSIS code that
references `SystemCoreClock` links; call `SystemCoreClockUpdate()` to sync it
with the actual clock if you use it. The `device/` headers come from Keil's
`Keil.TM4C_DFP` and ARM's `ARM.CMSIS` packs (see `LICENSE`).

## Prerequisites

```sh
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib lm4flash   # CachyOS/Arch
```

TivaWare isn't vendored (TI's license forbids redistributing it). The Makefile
defaults `TIVAWARE_PATH` to the standard TI install location
(`~/ti/TivaWare_C_Series-2.2.0.295`). If yours lives elsewhere or is a different
version, override it:

```sh
make TIVAWARE_PATH=/opt/TivaWare_C_Series-2.2.0.295
```

If the path doesn't point at a real install, `make` stops with a clear error.

Clone with submodules to get `external/printf`:

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

The vendored `external/printf` and `startup_gcc.c` are never touched.
