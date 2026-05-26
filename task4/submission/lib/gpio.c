#include "gpio.h"
#include "../ch32fun/ch32fun/ch32fun.h"
#include "../ch32fun/ch32fun/ch32v003hw.h"

void gpio_init(uint8_t pin) {
  RCC->APB2PCENR |= RCC_IOPDEN;
  GPIOD->CFGLR &= ~(0xF << (pin * 4));
  GPIOD->CFGLR |= (0x3 << (pin * 4));
  gpio_clear(pin);
}

void gpio_set(uint8_t pin) { GPIOD->BSHR = (1 << pin); }
void gpio_clear(uint8_t pin) { GPIOD->BCR = (1 << pin); }

void gpio_toggle(uint8_t pin) {
  if (GPIOD->OUTDR & (1 << pin))
    gpio_clear(pin);
  else
    gpio_set(pin);
}

uint8_t gpio_read(uint8_t pin) { return (GPIOD->INDR & (1 << pin)) ? 1 : 0; }
