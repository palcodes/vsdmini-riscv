#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// Base addresses for GPIO and RCC
#define GPIOD_BASE 0x40011400
#define RCC_BASE 0x40021000

// Register offsets
#define GPIOD_CFGLR (*(volatile uint32_t *)(GPIOD_BASE + 0x00))
#define GPIOD_OUTDR (*(volatile uint32_t *)(GPIOD_BASE + 0x0C))
#define GPIOD_BSHR (*(volatile uint32_t *)(GPIOD_BASE + 0x10))
#define GPIOD_BCR (*(volatile uint32_t *)(GPIOD_BASE + 0x14))

// RCC offsets
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB2ENR_IOPDEN (1 << 5)

#define UART_TX_PIN 5
#define LED_PIN 6

void gpio_init(void);
void gpio_set(void);
void gpio_clear(void);
void gpio_toggle(void);

#endif
