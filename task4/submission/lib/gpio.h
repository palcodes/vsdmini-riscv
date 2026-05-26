#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init(uint8_t pin);
void gpio_set(uint8_t pin);     //  LED on (drives pin LOW, active-low)
void gpio_clear(uint8_t pin);   //  LED off (drives pin HIGH)
void gpio_toggle(uint8_t pin);
uint8_t gpio_read(uint8_t pin);

#endif
