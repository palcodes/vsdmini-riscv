#include "../ch32fun/ch32fun/ch32fun.h"

#include "../library/timer.h"
#include "../library/gpio.h"
#include "../library/uart.h"

int main(void)
{

    SystemInit();

    gpio_init(6); // Initialize GPIO pin 6 for the LED
    uart_init(); // Initialize UART for sending time log
    timer_init();

    __enable_irq(); // Enable global interrupts

    uart_send_string("VSDSquadron Mini | Firmware v1.0\r\n");

    while (1)
    {
        gpio_toggle(6); // Toggle LED at PIN 6

        uint32_t tick = get_tick();
        uart_send_string("Tick: ");
        uart_send_number(tick);
        uart_send_string("\r\n");

        delay_ms(1000); // Delay for 1 second);
    }
}
