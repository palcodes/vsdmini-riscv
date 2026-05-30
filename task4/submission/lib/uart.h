#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @brief Initializes USART1 for 115200 8N1 communication.
 *
 * Enables RCC clocks for USART1 and GPIOD. Configures PD5 as
 * alternate-function push-pull (TX) and PD6 as floating input (RX).
 * Sets baud rate register for 115200 at system clock. Enables TX and RX.
 *
 * @return void
 *
 * @note Must be called before any other uart_* function.
 */
void uart_init(void);

/**
 * @brief Transmits a single character over USART1.
 *
 * Blocks until the TX data register is empty (TXE flag set),
 * then writes @p c to USART1->DATAR.
 *
 * @param c  Character to transmit.
 * @return   void
 *
 * @note Blocking. Do not call from an ISR context.
 */
void uart_send_char(char c);

/**
 * @brief Transmits a null-terminated string over USART1.
 *
 * Iterates over @p s and calls uart_send_char() for each byte.
 * Stops at the null terminator.
 *
 * @param s  Pointer to null-terminated string. Must not be NULL.
 * @return   void
 *
 * @note Blocking. String must be null-terminated.
 */
void uart_send_string(const char *s);

/**
 * @brief Transmits a 32-bit unsigned integer as a decimal ASCII string.
 *
 * Converts @p n to decimal digits and transmits them most-significant first.
 * Handles n == 0 as a special case (transmits "0"). Does not append newline.
 *
 * @param n  Unsigned 32-bit value to transmit.
 * @return   void
 *
 * @note Blocking. No trailing newline or whitespace is added.
 */
void uart_send_number(uint32_t n);

/**
 * @brief Blocks until a character is received and returns it.
 *
 * Polls the RXNE flag on USART1->STATR. Reads from USART1->DATAR.
 * Normalizes '\r' to '\n' for cross-platform terminal compatibility.
 *
 * @return  Received character. '\r' is returned as '\n'.
 *
 * @note Blocking indefinitely. Not suitable for timeout-sensitive paths.
 */
char uart_read_char(void);

/**
 * @brief Non-blocking check for pending received data.
 *
 * Reads the RXNE bit from USART1->STATR. Does not consume the byte.
 *
 * @return  1 if data is available in the RX register, 0 otherwise.
 *
 * @note Follow with uart_read_char() to fetch the pending byte.
 */
uint8_t uart_data_ready(void);

#endif
