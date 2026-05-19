#ifndef UART_H
#define UART_H

#include "gpio.h"
#include <stdint.h>

// Register bases
#define UART_BASE 0x40013800

// Register offsets
#define UART_SR (*(volatile uint32_t *)(UART_BASE + 0x00))
#define UART_DR (*(volatile uint32_t *)(UART_BASE + 0x04))
#define UART_BRR (*(volatile uint32_t *)(UART_BASE + 0x08))
#define UART_CR1 (*(volatile uint32_t *)(UART_BASE + 0x0C))

#define UART_CR1_UE (1 << 13)
#define UART_CR1_TE (1 << 3)
#define UART_SR_TXE (1 << 7)

// Clock Enable for UART and AFIO
#define RCC_APB2ENR_UARTEN (1 << 14)
#define RCC_APB2ENR_AFIOEN (1 << 0)

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(const char *s);

#endif
