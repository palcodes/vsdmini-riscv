#include <stdint.h>

#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(const char *s);
void uart_send_number(uint32_t n);
char uart_read_char(void);
uint8_t uart_data_ready(void);

#endif
