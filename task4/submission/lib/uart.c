#include "uart.h"
#include "../ch32fun/ch32fun/ch32fun.h"
#include "../ch32fun/ch32fun/ch32v003hw.h"

void uart_init(void) {
    RCC->APB2PCENR |= RCC_USART1EN | RCC_IOPDEN;

    GPIOD->CFGLR &= ~(0xF << (5 * 4));
    GPIOD->CFGLR |=  (0xB << (5 * 4));

    GPIOD->CFGLR &= ~(0xF << (6 * 4));
    GPIOD->CFGLR |=  (0x4 << (6 * 4));

    USART1->BRR = 0x1A1;
    USART1->CTLR1 |= USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_UE;
}

void uart_send_char(char c) {
    while (!(USART1->STATR & USART_STATR_TXE));
    USART1->DATAR = c;
}

void uart_send_string(const char *s) {
    while (*s) uart_send_char(*s++);
}

void uart_send_number(uint32_t n) {
    char buf[10];
    int i = 0;
    if (n == 0) { uart_send_char('0'); return; }
    while (n > 0) { buf[i++] = '0' + (n % 10); n /= 10; }
    while (i--) uart_send_char(buf[i]);
}

char uart_read_char(void) {
    while (!(USART1->STATR & USART_STATR_RXNE));
    char c = (char)USART1->DATAR;
    return (c == '\r') ? '\n' : c;
}

uint8_t uart_data_ready(void) {
    return (USART1->STATR & USART_STATR_RXNE) ? 1 : 0;
}
