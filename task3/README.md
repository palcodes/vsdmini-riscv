# Timer Driver Library + Demo Application

**Board:** VSDSquadron Mini (CH32V003F4U6, RISC-V)  
**Toolchain:** riscv64-unknown-elf-gcc + ch32fun  

---

## Library: Timer (TIM2)

Bare-metal TIM2-based timer library. No HAL. Register-level access only.

### API

| Function                     | Description                                                                |
| ---------------------------- | -------------------------------------------------------------------------- |
| `void timer_init(void)`      | Configures TIM2 for 1ms tick (PSC=47, ARR=999 at 48MHz), enables interrupt |
| `void delay_ms(uint32_t ms)` | Blocking delay using tick counter                                          |
| `uint32_t get_tick(void)`    | Returns elapsed milliseconds since `timer_init()`                          |

**Interrupt:** `TIM2_IRQHandler` increments internal `ticks` counter every 1ms.

---

## Demo Application

Blinks the onboard LED (PD6) every 1 second and streams a timestamped log over UART.

**Output sample:**
```
VSDSquadron Mini | Firmware v1.0
Tick: 1000
Tick: 2000
Tick: 3000
```

**Libraries used:** Timer + GPIO + UART — all register-level exposed through CH32FUN libray.

---

## Build & Flash

### Prerequisites
- `riscv64-unknown-elf-gcc`
- `libusb-1.0-dev`, `libudev-dev`
- WCH-LinkE programmer

### Build
```bash
cd task3
make
```

### Flash
Connect WCH-LinkE, then:
```bash
make flash
```

---

## UART Configuration

| Parameter | Value          |
| --------- | -------------- |
| Port      | `/dev/ttyACM0` |
| Baud rate | 115200         |
| Data bits | 8              |
| Stop bits | 1              |
| Parity    | None           |

Monitor:
```bash
screen /dev/ttyACM0 115200
```