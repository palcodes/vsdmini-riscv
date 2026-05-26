# Evidence — UART Command Console + GPIO Control Panel

## 1. UART Logs

<img height="400" alt="image" src="submission/media/uart_console_scrncast.gif" />

```
<!-- UART Console Terminal TX/RX

VSDSquadron Mini | UART CLI v1.0

Commands:
  help          - show this menu
  led on        - turn LED on
  led off       - turn LED off
  blink <ms>    - blink LED every <ms> milliseconds
  read <pin>    - read GPIOD pin state
  stop          - stop blinking

> led on
LED turned on
> read 4
PD4 = 1
> led off
LED turned off
> read 4
PD4 = 0
> blink 500
Blinking every 500ms
> blink 200
Blinking every 200ms
> blink 2000
Blinking every 2000ms
> stop
Stopped
> askdslsd
Unknown command. Type 'help'
> help

Commands:
  help          - show this menu
  led on        - turn LED on
  led off       - turn LED off
  blink <ms>    - blink LED every <ms> milliseconds
  read <pin>    - read GPIOD pin state
  stop          - stop blinking
-->
```

---

## 2. Hardware Proof

<img height="380" alt="image" src="submission/media/led_vid.gif" />

> - External LED wired to PD4 via breadboard
> - LED visibly on or blinking
> - USB-C connected to host machine

---

## 3. Verification Notes

### What was tested
- [x] `help` — prints all commands
- [x] `led on` — LED turns on
- [x] `led off` — LED turns off
- [x] `blink 500` — LED blinks every 500ms, non-blocking (CLI still accepts input)
- [x] `blink 100` — LED blinks faster
- [x] `stop` — blink stops, LED turns off
- [x] `read 4` — prints PD4 pin state
- [x] Unknown command — prints error message
- [x] Empty enter — no crash, re-prompts `>`
- [x] Backspace — correctly erases character in terminal
