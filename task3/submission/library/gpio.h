#include <stdint.h>

#ifndef GPIO_H
#define GPIO_H

void gpio_init(uint8_t pin);
void gpio_set(uint8_t pin);
void gpio_clear(uint8_t pin);
void gpio_toggle(uint8_t pin);

#endif
