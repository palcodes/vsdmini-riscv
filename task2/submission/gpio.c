#include "gpio.h"

// Initialize the GPIO pin
void gpio_init()
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPDEN; // Enable clock for GPIOD

    //  Clear bits [27:24] then set MODE=11, CNF=00 for PD6
    GPIOD_CFGLR &= ~(0xF << (LED_PIN * 4)); // Clear the configuration for the pin
    GPIOD_CFGLR |= (0x3 << (LED_PIN * 4));
}

// Set the GPIO pin high
void gpio_set()
{
    GPIOD_BSHR = (1 << LED_PIN); // Set the pin high
}

// Set the GPIO pin low
void gpio_clear()
{
    GPIOD_BCR = (1 << LED_PIN); // Set the pin low
}

// Toggle the GPIO pin
void gpio_toggle()
{
    if (GPIOD_OUTDR & (1 << LED_PIN))
    {
        gpio_clear(); // If the pin is high, set it low
    }
    else
    {
        gpio_set(); // If the pin is low, set it high
    }
}
