#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/**
 * @brief Configures a GPIOD pin as output push-pull at 50 MHz.
 *
 * Enables RCC clock for GPIOD and sets the CNF/MODE bits in GPIOD->CFGLR
 * for the specified pin. Pin is initialized LOW after configuration.
 *
 * @param pin  Pin number on GPIOD [0–7].
 * @return     void
 *
 * @note Must be called before gpio_set, gpio_clear, gpio_toggle, or gpio_read.
 *       Only configures output mode. Call once per pin at startup.
 */
void gpio_init(uint8_t pin);

/**
 * @brief Drives a GPIOD output pin HIGH (LED on, active HIGH).
 *
 * Writes to the set half of GPIOD->BSHR. Atomic — does not affect other pins.
 *
 * @param pin  Pin number on GPIOD [0–7].
 * @return     void
 *
 * @note Pin must be initialized as output via gpio_init() first.
 */
void gpio_set(uint8_t pin);

/**
 * @brief Drives a GPIOD output pin LOW (LED off).
 *
 * Writes to the reset half of GPIOD->BSHR (upper 16 bits). Atomic.
 *
 * @param pin  Pin number on GPIOD [0–7].
 * @return     void
 *
 * @note Pin must be initialized as output via gpio_init() first.
 */
void gpio_clear(uint8_t pin);

/**
 * @brief Toggles the output state of a GPIOD pin.
 *
 * Reads current state from GPIOD->OUTDR, inverts the target bit,
 * and writes back via BSHR to keep other pins unaffected.
 * Called from TIM2 ISR during blink operation.
 *
 * @param pin  Pin number on GPIOD [0–7].
 * @return     void
 *
 * @note Pin must be initialized as output via gpio_init() first.
 */
void gpio_toggle(uint8_t pin);

/**
 * @brief Reads the current state of a GPIOD pin from the input data register.
 *
 * Reads GPIOD->INDR and extracts the bit at position @p pin.
 * For output pins, reflects the driven state. Verified empirically:
 * gpio_set(4) followed by gpio_read(4) returns 1 (active HIGH confirmed).
 *
 * @param pin  Pin number on GPIOD [0–7].
 * @return     1 if pin is HIGH, 0 if pin is LOW.
 */
uint8_t gpio_read(uint8_t pin);

#endif
