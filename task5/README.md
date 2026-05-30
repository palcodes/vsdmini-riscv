# Changelog

All notable changes to this project are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## v1.0.0 — Initial Release

### Added

- **UART driver** — USART1, PD5 TX / PD6 RX, 115200 8N1
  - `uart_init`, `uart_send_char`, `uart_send_string`, `uart_send_number`
  - `uart_read_char` with `\r` → `\n` normalization
  - `uart_data_ready` for non-blocking RXNE poll

- **GPIO driver** — GPIOD register-level control
  - `gpio_init`, `gpio_set`, `gpio_clear`, `gpio_toggle`, `gpio_read`
  - Active HIGH verified on PD4 via INDR read-back

- **Timer driver** — TIM2, 1ms update interrupt
  - `timer_init`, `delay_ms`, `get_tick`
  - Tick counter incremented in TIM2 ISR

- **CLI application** — UART command console
  - Commands: `led on`, `led off`, `blink <ms>`, `stop`, `read <pin>`, `help`
  - Non-blocking blink via TIM2 ISR (`blink_active` + `blink_period_ms` globals)
  - Backspace handling (0x7F and 0x08)
  - Input buffer overflow protection (64-byte cap)
  - Edge case handling: empty input, invalid arguments, unknown commands
  - Boot banner and help menu on reset

### Hardware

- Target: VSDSquadron Mini (CH32V003F4U6, RISC-V RV32EC)
- LED: external, wired to PD4 via 220Ω resistor, active HIGH
- Build system: ch32fun Makefile toolchain, xPack riscv-none-elf-gcc
