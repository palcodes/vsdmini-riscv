# API Reference

Driver APIs for the UART Command Console project.
All drivers target the CH32V003F4U6 and operate at register level.
No return-value error codes are used unless the function signature explicitly
declares one, void functions are fire-and-forget.

---

## Table of Contents

- [UART Driver](#uart-driver)
- [GPIO Driver](#gpio-driver)
- [Timer Driver](#timer-driver)

---

## UART Driver

**File:** `lib/uart.c` / `lib/uart.h`  
**Peripheral:** USART1  
**Pins:** PD5 (TX), PD6 (RX)  
**Configuration:** 115200 baud, 8N1, no flow control

---

### `uart_init`

```c
void uart_init(void);
```

**Brief:** Initializes USART1 for 115200 8N1 communication.

**Details:**
- Enables RCC clock for USART1 and GPIOD
- Configures PD5 as alternate-function push-pull (TX)
- Configures PD6 as floating input (RX)
- Sets baud rate register for 115200 at system clock
- Enables USART1 TX and RX

**Parameters:** None  
**Returns:** void  
**Constraints:** Must be called before any other uart_* function.

**Example:**
```c
uart_init();
uart_send_string("Boot OK\r\n");
```

---

### `uart_send_char`

```c
void uart_send_char(char c);
```

**Brief:** Transmits a single character over USART1.

**Details:**
- Blocks until the TX data register is empty (TXE flag)
- Writes character to USART1->DATAR

**Parameters:**
- `c` — character to transmit

**Returns:** void  
**Constraints:** Blocking. Do not call from an ISR context.

**Example:**
```c
uart_send_char('A');
```

---

### `uart_send_string`

```c
void uart_send_string(const char *s);
```

**Brief:** Transmits a null-terminated string over USART1.

**Details:**
- Iterates over `s` and calls `uart_send_char` for each byte
- Stops at null terminator

**Parameters:**
- `s` — pointer to null-terminated string; must not be NULL

**Returns:** void  
**Constraints:** Blocking. String must be null-terminated.

**Example:**
```c
uart_send_string("LED turned on\r\n");
```

---

### `uart_send_number`

```c
void uart_send_number(uint32_t n);
```

**Brief:** Transmits a 32-bit unsigned integer as a decimal ASCII string.

**Details:**
- Converts `n` to decimal digits via repeated modulo/divide
- Transmits digit characters in order, most significant first
- Handles `n == 0` as a special case (transmits `"0"`)

**Parameters:**
- `n` — unsigned 32-bit value to transmit

**Returns:** void  
**Constraints:** Blocking. Does not append newline.

**Example:**
```c
uart_send_string("Tick: ");
uart_send_number(get_tick());
uart_send_string("\r\n");
```

---

### `uart_read_char`

```c
char uart_read_char(void);
```

**Brief:** Blocks until a character is received and returns it.

**Details:**
- Polls RXNE (receive not empty) flag
- Reads from USART1->DATAR
- Normalizes `\r` to `\n` for cross-platform terminal compatibility

**Parameters:** None  
**Returns:** Received character (`\r` is returned as `\n`)  
**Constraints:** Blocking indefinitely. Not suitable for timeout-sensitive loops.

**Example:**
```c
char c = uart_read_char();
if (c == '\n') process_command();
```

---

### `uart_data_ready`

```c
int uart_data_ready(void);
```

**Brief:** Non-blocking check for pending received data.

**Details:**
- Reads RXNE bit from USART1->STATR
- Returns immediately without consuming the byte

**Parameters:** None  
**Returns:**
- `1` — data is available in RX register
- `0` — no data pending

**Constraints:** Does not read or consume the byte. Follow with `uart_read_char` to fetch it.

**Example:**
```c
if (uart_data_ready()) {
    char c = uart_read_char();
}
```

---

## GPIO Driver

**File:** `lib/gpio.c` / `lib/gpio.h`  
**Peripheral:** GPIOD  
**LED pin:** PD4, output push-pull, active HIGH

---

### `gpio_init`

```c
void gpio_init(uint8_t pin);
```

**Brief:** Configures a GPIOD pin as output push-pull at 50 MHz.

**Details:**
- Enables RCC clock for GPIOD if not already enabled
- Sets CNF/MODE bits in GPIOD->CFGLR for the specified pin
- Initializes pin to LOW state

**Parameters:**
- `pin` — pin number (0–7) on GPIOD

**Returns:** void  
**Constraints:** Only configures output mode. Separate init required for input pins.

**Example:**
```c
gpio_init(4);  // Configure PD4 as output
```

---

### `gpio_set`

```c
void gpio_set(uint8_t pin);
```

**Brief:** Drives a GPIOD output pin HIGH.

**Details:**
- Writes to GPIOD->BSHR (bit set/reset register), set half
- Atomic single-cycle operation; does not require read-modify-write

**Parameters:**
- `pin` — pin number (0–7) on GPIOD

**Returns:** void  
**Constraints:** Pin must be initialized as output via `gpio_init` first.

**Example:**
```c
gpio_set(4);  // LED on
```

---

### `gpio_clear`

```c
void gpio_clear(uint8_t pin);
```

**Brief:** Drives a GPIOD output pin LOW.

**Details:**
- Writes to GPIOD->BSHR reset half (upper 16 bits)

**Parameters:**
- `pin` — pin number (0–7) on GPIOD

**Returns:** void  
**Constraints:** Pin must be initialized as output via `gpio_init` first.

**Example:**
```c
gpio_clear(4);  // LED off
```

---

### `gpio_toggle`

```c
void gpio_toggle(uint8_t pin);
```

**Brief:** Toggles the output state of a GPIOD pin.

**Details:**
- Reads current output state from GPIOD->OUTDR
- Inverts the target bit
- Writes back via BSHR to maintain atomicity on other pins

**Parameters:**
- `pin` — pin number (0–7) on GPIOD

**Returns:** void  
**Constraints:** Pin must be initialized as output.

**Example:**
```c
gpio_toggle(4);  // Called from TIM2 ISR during blink
```

---

### `gpio_read`

```c
uint8_t gpio_read(uint8_t pin);
```

**Brief:** Reads the current input state of a GPIOD pin.

**Details:**
- Reads GPIOD->INDR (input data register)
- Extracts and returns the bit at position `pin`

**Parameters:**
- `pin` — pin number (0–7) on GPIOD

**Returns:**
- `1` — pin is HIGH
- `0` — pin is LOW

**Constraints:**
- For output pins, reflects the driven state (verified empirically for PD4).
- Active HIGH: `gpio_read(4) == 1` means LED is on.

**Example:**
```c
uint8_t state = gpio_read(4);
uart_send_string("PD4 = ");
uart_send_number(state);
uart_send_string("\r\n");
```

---

## Timer Driver

**File:** `lib/timer.c` / `lib/timer.h`  
**Peripheral:** TIM2  
**Tick period:** 1 ms  
**IRQ:** TIM2 update interrupt

---

### `timer_init`

```c
void timer_init(void);
```

**Brief:** Configures TIM2 to generate a 1 ms update interrupt.

**Details:**
- Enables RCC clock for TIM2
- Sets prescaler and auto-reload register for 1 ms period at system clock
- Enables update interrupt (UIE) in TIM2->DMAINTENR
- Enables TIM2 IRQ in PFIC (CH32V003 interrupt controller)
- Starts the counter

**Parameters:** None  
**Returns:** void  
**Constraints:** Must be called before `delay_ms` or `get_tick`. Call once at startup.

**Example:**
```c
timer_init();
delay_ms(500);
```

---

### `delay_ms`

```c
void delay_ms(uint32_t ms);
```

**Brief:** Blocking delay using the TIM2 millisecond tick.

**Details:**
- Records `get_tick()` at entry
- Spins until `get_tick() - start >= ms`
- Does not disable interrupts; other ISRs continue to fire

**Parameters:**
- `ms` — delay duration in milliseconds

**Returns:** void  
**Constraints:**
- Blocking. Not suitable for use inside an ISR.
- Requires `timer_init()` to have been called.
- Maximum reliable delay bounded by `uint32_t` rollover (~49 days).

**Example:**
```c
gpio_set(4);
delay_ms(1000);
gpio_clear(4);
```

---

### `get_tick`

```c
uint32_t get_tick(void);
```

**Brief:** Returns the number of milliseconds elapsed since `timer_init` was called.

**Details:**
- Reads a `volatile uint32_t` tick counter incremented by the TIM2 ISR
- No critical section required on 32-bit aligned read on RV32EC

**Parameters:** None  
**Returns:** Milliseconds since boot as `uint32_t`  
**Constraints:** Rolls over after ~49.7 days. Application does not run long enough for this to matter.

**Example:**
```c
uint32_t t0 = get_tick();
// ... do work ...
uint32_t elapsed = get_tick() - t0;
```

---

## TIM2 ISR — Blink Handler

The TIM2 update ISR is defined in `main.c` (not a public API, documented here for completeness).

```c
void TIM2_IRQHandler(void) __attribute__((interrupt));
void TIM2_IRQHandler(void) {
    if (TIM2->INTFR & TIM_UIF) {
        tick_ms++;
        if (blink_active && (tick_ms % blink_period_ms == 0)) {
            gpio_toggle(LED_PIN);
        }
        TIM2->INTFR &= ~TIM_UIF;  // Clear update flag
    }
}
```

**Behavior:**
- Increments global `tick_ms` every 1 ms
- If `blink_active` is set, toggles LED when `tick_ms` is divisible by `blink_period_ms`
- Clears the update interrupt flag before returning

**Globals modified:** `tick_ms` (volatile), `blink_active` (volatile), reads `blink_period_ms` (volatile)
