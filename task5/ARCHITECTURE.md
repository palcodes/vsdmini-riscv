# Architecture

System design for the UART Command Console + GPIO Control Panel.

### Driver Layer (`lib/`)

Each driver is a self-contained module:

| Module    | Responsibility     | Peripheral |
| --------- | ------------------ | ---------- |
| `uart.c`  | Byte-level TX/RX   | USART1     |
| `gpio.c`  | Pin set/clear/read | GPIOD      |
| `timer.c` | 1ms tick, delay    | TIM2       |

Drivers do not call each other. They expose flat C APIs and operate directly on
peripheral registers via ch32fun's CMSIS-style `->` accessor macros.

### Application Layer (`app/main.c`)

Owns all state and orchestration:

- Calls `uart_read_char()` to assemble a command string
- Calls `parse_command()` to identify the command token and argument
- Dispatches to `execute_command()` which mutates driver state
- TIM2 ISR lives here — modifies `tick_ms`, `blink_active`, `blink_period_ms`

---

## Data Flow

### Command Input Path

```
Host keyboard keystroke
    │
    ▼ (USB-UART bridge)
USART1 RX register (RXNE flag set)
    │
    ▼ uart_read_char()
Character accumulated into cmd_buf[]
    │ (on '\n')
    ▼ parse_command()
Token + argument extracted
    │
    ▼ execute_command()
gpio_set() / gpio_clear() / blink state update
    │
    ▼ uart_send_string()
Response string sent back to host
```

### Blink Control Path

```
execute_command("blink 500")
    │
    ├── blink_period_ms = 500
    └── blink_active = 1
              │
              ▼ (every 1ms)
        TIM2 IRQ fires
              │
              ▼ tick_ms % blink_period_ms == 0 ?
              │   yes → gpio_toggle(LED_PIN)
              │   no  → return
              ▼
        LED state changes without blocking main loop
```

### Read Pin Path

```
execute_command("read 4")
    │
    ▼ gpio_read(4)       ← reads GPIOD->INDR
    │
    ▼ uart_send_string("PD4 = ")
      uart_send_number(state)
```

---

## Design Decisions

**Why no RTOS or cooperative scheduler?**  
The application has one foreground task (CLI) and one time-driven background
task (blink). TIM2 IRQ covers the background task with negligible overhead.
A scheduler would add complexity with no benefit at this scale.

**Why is the TIM2 ISR in main.c instead of timer.c?**  
The ISR references application-level globals (`blink_active`, `blink_period_ms`).
Placing it in `timer.c` would either couple the driver to the application or
require callbacks. For a single-application project, co-location in `main.c`
is simpler and avoids over-engineering.

**Why poll RXNE instead of using UART RX interrupt?**  
The CLI loop is inherently sequential: read char → buffer → process on newline.
An RX IRQ would require a ring buffer and critical sections for no latency
benefit at 115200 baud and human typing speeds.
