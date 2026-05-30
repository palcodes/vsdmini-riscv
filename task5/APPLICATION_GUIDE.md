# Application Guide

Describes the CLI application logic in `app/main.c`.
This document covers state, command parsing, timing, and edge case handling.
For driver internals, see `API_REFERENCE.md`.

---

## Application States

The application operates in two orthogonal states simultaneously:

### LED State
| State | LED | blink_active |
|---|---|---|
| OFF | LOW | 0 |
| ON (steady) | HIGH | 0 |
| BLINKING | Toggling | 1 |

`led on` and `led off` set `blink_active = 0` before driving the pin, ensuring
blink cannot re-override a manual command within the same tick window.

### CLI State
The CLI loop does not block on command execution. It blocks only on character
input (`uart_read_char`), which is acceptable since the blink ISR runs independently.

---

## Startup Sequence

```
main()
  │
  ├── SystemInit()       ← ch32fun clock setup
  ├── timer_init()       ← TIM2 1ms tick + IRQ
  ├── uart_init()        ← USART1 115200 8N1
  ├── gpio_init(4)       ← PD4 output, initially LOW
  │
  ├── uart_send_string() ← Banner: "VSDSquadron Mini | UART CLI v1.0"
  ├── print_help()       ← Command menu
  │
  └── cli_loop()         ← Enter main loop
```

---

## Command Parsing

Input is accumulated character by character into `cmd_buf[64]`.

On receipt of `\n` (or `\r` normalized to `\n` by `uart_read_char`):

1. Null-terminate `cmd_buf`
2. Strip leading/trailing whitespace
3. Extract first token (command word)
4. Extract remainder as argument string (if any)

```
Input:  "blink 500\n"
Token:  "blink"
Arg:    "500"
```

Argument is converted to `uint32_t` via `simple_atoi()` (local implementation,
does not use stdlib).

---

## Command Dispatch Table

| Input token | Action |
|---|---|
| `led on` | `blink_active = 0`, `gpio_set(LED_PIN)` |
| `led off` | `blink_active = 0`, `gpio_clear(LED_PIN)` |
| `blink <ms>` | validate arg, set `blink_period_ms`, set `blink_active = 1` |
| `stop` | `blink_active = 0`, `gpio_clear(LED_PIN)` |
| `read <pin>` | `gpio_read(pin)`, print result |
| `help` | `print_help()` |
| (empty) | re-prompt, no action |
| (unknown) | print `"Unknown command. Type 'help'"` |

---

## Timing Behavior

### Blink Mechanism

TIM2 fires every 1 ms. Inside the ISR:

```c
tick_ms++;
if (blink_active && (tick_ms % blink_period_ms == 0)) {
    gpio_toggle(LED_PIN);
}
```

The LED toggles when `tick_ms` is an exact multiple of `blink_period_ms`.
This gives a full blink period (on + off) of `2 × blink_period_ms` wall-clock
milliseconds. A `blink 500` command produces a 1 Hz blink (500ms on, 500ms off).

### Blink Period Update

Updating `blink_period_ms` mid-blink takes effect on the next ISR evaluation.
No reset of `tick_ms` occurs; the new period aligns to the next matching
multiple. This produces at most one extended or shortened half-cycle on
period change, which is acceptable for this application.

### CLI Latency During Blink

The TIM2 ISR is ~5 instructions. At 48 MHz, this is sub-microsecond. CLI input
polling is unaffected. `uart_read_char` blocking on RXNE is not interrupted by
the blink ISR in a way that would drop characters.

---

## Edge Cases Handled

| Input | Behavior |
|---|---|
| Empty line (Enter only) | Silently re-prompts `>` |
| `blink 0` | Rejected — prints `"Invalid period"`, no state change |
| `blink` (no argument) | Rejected — prints `"Usage: blink <ms>"` |
| `read` (no argument) | Rejected — prints `"Usage: read <pin>"` |
| `read 9` (pin > 7) | Rejected — prints `"Invalid pin"` |
| Unknown token | Prints `"Unknown command. Type 'help'"` |
| Backspace (`0x7F` / `0x08`) | Decrements buffer index, echoes backspace+space+backspace to erase character in terminal |
| Buffer overflow (>63 chars) | Input is silently truncated; null terminator enforced |
| `blink <ms>` → `led on` | `blink_active = 0` set before `gpio_set`, ISR cannot re-toggle |

---

## Backspace Handling Detail

Terminal emulators send `0x7F` (DEL) or `0x08` (BS) for backspace depending on
configuration. The CLI handles both:

```c
if (c == 0x7F || c == 0x08) {
    if (buf_idx > 0) {
        buf_idx--;
        uart_send_string("\b \b");  // erase character visually
    }
    continue;
}
```

This is necessary because USART1 is in raw mode with no line discipline.
