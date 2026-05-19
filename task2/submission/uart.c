#include "uart.h"
#include "gpio.h"

void uart_init() {
  // Enable RCC Clock for UART
  RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_UARTEN; // Bit 0 and Bit 14

  // Configure PD5
  GPIOD_CFGLR &= ~(0xF << (UART_TX_PIN * 4));
  GPIOD_CFGLR |= (0xB << (UART_TX_PIN * 4));

  // Set BRR for 115200 baud at 48 MHz
  UART_BRR = 24000000 / 115200;


  // Enable TE and UE in CR1
  UART_CR1 |= UART_CR1_TE | UART_CR1_UE;
}


void uart_send_char(char c) {
  while (!(UART_SR & UART_SR_TXE));
  UART_DR = c;
}

void uart_send_string(const char *s) {
  while (*s) {
    uart_send_char(*s++);
  }
}
