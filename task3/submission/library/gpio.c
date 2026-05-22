#include "../ch32fun/ch32fun/ch32fun.h"
#include "../ch32fun/ch32fun/ch32v003hw.h"

void gpio_init(uint8_t pin)
{
    RCC->APB2PCENR |= RCC_IOPDEN; // Enable clock for GPIOD

    GPIOD->CFGLR &= ~(0xF << (pin * 4)); // Clear bits for the specified pin
    GPIOD->CFGLR |= (0x3 << (pin * 4));  // Set bits for the specified pin
}

void gpio_set(uint8_t pin)
{
    GPIOD->BCR = (1 << pin); // Set the specified pin (active low)
}

void gpio_clear(uint8_t pin)
{
    GPIOD->BSHR = (1 << pin); // Clear the specified pin (active low)
}

void gpio_toggle(uint8_t pin)
{
    if (GPIOD->OUTDR & (1 << pin))
    {
        gpio_set(pin);
    }
    else
    {
        gpio_clear(pin);
    }
}