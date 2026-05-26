# UART Command Console + GPIO Control Panel

A bare-metal embedded CLI application for the VSDSquadron Mini (CH32V003F4U6).
Commands sent over UART control an external LED via GPIO, with timer-driven
non-blocking blink support. All logic is built on custom register-level drivers
with no HAL dependencies beyond ch32fun.

---

## Drivers Used

- **UART** — USART1, PD5 TX / PD6 RX, 115200 8N1
- **GPIO** — GPIOD, PD4 output (external LED), active HIGH
- **Timer** — TIM2, 1ms tick via IRQ, used for non-blocking blink

---

## API Summary

### UART

| Function | Description |
|---|---|
| `uart_init()` | Init USART1, PD5 TX, PD6 RX |
| `uart_send_char(c)` | Blocking TX single char |
| `uart_send_string(s)` | Blocking TX string |
| `uart_send_number(n)` | TX uint32 as decimal |
| `uart_read_char()` | Blocking RX, normalizes `\r` → `\n` |
| `uart_data_ready()` | Non-blocking RXNE poll |

### GPIO

| Function | Description |
|---|---|
| `gpio_init(pin)` | Output push-pull, 50MHz |
| `gpio_set(pin)` | Drive HIGH |
| `gpio_clear(pin)` | Drive LOW |
| `gpio_toggle(pin)` | Toggle output |
| `gpio_read(pin)` | Read INDR |

### Timer

| Function | Description |
|---|---|
| `timer_init()` | TIM2, 1ms tick, IRQ enabled |
| `delay_ms(ms)` | Blocking delay |
| `get_tick()` | Returns ms since boot |

---

## Folder Structure

```
task4/submission/
├── lib/
│   ├── gpio.c / gpio.h
│   ├── uart.c / uart.h
│   └── timer.c / timer.h
├── app/
│   └── main.c
├── README.md
└── evidence.md
```

---

## Build + Flash

```bash
cd task4/submission
make
```

---

## UART Settings

- **Baud:** 115200
- **Port:** /dev/ttyACM0 (or ACM1, check with `ls /dev/ttyACM*`)
- **Format:** 8N1

Connect [linux]:

```bash
screen /dev/ttyACM0 115200
```
[mac]:

```bash
screen /dev/cu.* 115200
```
---

## Demo

1. Flash firmware and connect USB-C
2. Open terminal with screen
3. Press reset on board — banner and help menu appear
4. Run commands:

| Command | Expected Result |
|---|---|
| `led on` | LED turns on |
| `led off` | LED turns off |
| `blink 500` | LED blinks every 500ms |
| `blink 100` | LED blinks every 100ms |
| `stop` | Stops blinking, LED off |
| `read 4` | Prints PD4 pin state |
| `help` | Prints command menu |
| `asdf` | Prints unknown command error |

---
