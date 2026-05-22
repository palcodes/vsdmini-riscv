#include "../ch32fun/ch32fun/ch32fun.h"

#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/uart.h"

int main(void) {

  SystemInit();

  gpio_init(6); // Initialize GPIO pin 6 for the LED
  uart_init();  // Initialize UART for sending time log
  timer_init();


  uart_send_string("VSDSquadron Mini | Firmware v1.0\r\n");

  while (1) {
    gpio_toggle(6);
    uart_send_string("Uptime: ");
    uart_send_number(get_tick() / 1000);
    uart_send_string("s\r\n");
    delay_ms(1000);
  }
}
