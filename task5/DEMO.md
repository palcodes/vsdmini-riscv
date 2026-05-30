# Demo Guide

Reproduces the full demo in under 5 minutes.
Assumes: Linux or macOS host, USB-C cable, breadboard LED circuit assembled.

---

## Hardware Setup

### Components Required

| Item | Qty |
|---|---|
| VSDSquadron Mini | 1 |
| LED (any color) | 1 |
| 220Ω resistor | 1 |
| Breadboard + jumper wires | — |
| USB-C cable | 1 |

### Wiring

```
VSDSquadron Mini PD4 ──▶ 220Ω resistor ──▶ LED anode
LED cathode ──▶ GND
```

> LED is active HIGH. PD4 HIGH → LED on. PD4 LOW → LED off.

<!-- attach wiring photo here -->

---

## Build and Flash

```bash
# 1. Enter project directory
cd task4/submission

# 2. Build firmware
make

# 3. Connect USB-C to host. Board enumerates as /dev/ttyACM0 (Linux)
ls /dev/ttyACM*

# 4. Flash
make flash

# Expected make flash output:
# minichlink: found WCH-LinkE
# Wrote firmware to device
```

> If `make flash` fails with permission denied, add your user to the `plugdev` group:
> ```bash
> sudo usermod -aG plugdev $USER
> # Log out and log back in
> ```

---

## Open Serial Console

**Linux:**
```bash
screen /dev/ttyACM0 115200
```

**macOS:**
```bash
screen /dev/cu.usbmodem* 115200
```

UART settings: **115200 baud, 8N1, no flow control**.

To exit `screen`: `Ctrl+A` then `K`, confirm with `Y`.

---

## Expected Boot Output

Press the **RESET** button on the board after opening the terminal.

```
VSDSquadron Mini | UART CLI v1.0

Commands:
  help          - show this menu
  led on        - turn LED on
  led off       - turn LED off
  blink <ms>    - blink LED every <ms> milliseconds
  read <pin>    - read GPIOD pin state
  stop          - stop blinking

>
```

<!-- attach screenshot or GIF of boot output here -->

---

## Command Verification

Type each command and press Enter. Observe both terminal output and LED.

### `led on`
```
> led on
LED turned on
>
```
**LED:** ON (steady)

<!-- attach screenshot here -->

---

### `read 4`
```
> read 4
PD4 = 1
>
```
**Confirms:** GPIO INDR reflects driven HIGH state (active HIGH verified).

---

### `led off`
```
> led off
LED turned off
>
```
**LED:** OFF

---

### `read 4` (after led off)
```
> read 4
PD4 = 0
>
```

---

### `blink 500`
```
> blink 500
Blinking every 500ms
>
```
**LED:** Toggles every 500ms (1 Hz blink, 500ms on / 500ms off).  
**CLI:** Remains responsive — type another command while LED is blinking.

<!-- attach GIF of LED blinking here -->

---

### `blink 100`
```
> blink 100
Blinking every 100ms
>
```
**LED:** Visibly faster blink (5 Hz).

---

### `stop`
```
> stop
Stopped
>
```
**LED:** OFF. Blink halted.

---

### `help`
```
> help

Commands:
  help          - show this menu
  led on        - turn LED on
  led off       - turn LED off
  blink <ms>    - blink LED every <ms> milliseconds
  read <pin>    - read GPIOD pin state
  stop          - stop blinking

>
```

---

### Unknown command
```
> asdf
Unknown command. Type 'help'
>
```

---

### Empty input (Enter only)
```
>
>
```
No crash. Re-prompts cleanly.

---

### Backspace
Type a partial command, press Backspace, then correct it.
Characters erase from the terminal display. Buffer is correctly decremented.
No garbage in the executed command.

---

## Non-Blocking Blink Verification

Issue `blink 500`, then immediately type `help` without pressing stop.

```
> blink 500
Blinking every 500ms
> help            ← typed while LED is blinking

Commands:
  help          - show this menu
  ...

>
```

LED continues blinking while `help` output is printed. This confirms TIM2 ISR
drives blink independently of the CLI loop.

<!-- attach GIF showing simultaneous blink + CLI response here -->

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| No serial output after reset | Wrong port | Check `ls /dev/ttyACM*`, try ACM1 |
| Permission denied on flash | User not in plugdev | `sudo usermod -aG plugdev $USER` |
| LED does not turn on | Wiring | Verify PD4 → resistor → LED → GND |
| Characters doubled in terminal | Local echo on | `screen` has no local echo by default; use screen, not minicom default config |
| Garbled output | Wrong baud rate | Confirm 115200; mismatch causes framing errors |
