#include "gpio.h"
#include "uart.h"

static void itoa_simple(int n, char *buf) {
    int i = 0;
    if (n == 0) { buf[i++] = '0'; buf[i] = '\0'; return; }
    while (n > 0) { buf[i++] = '0' + (n % 10); n /= 10; }
    for (int a = 0, b = i-1; a < b; a++, b--) {
        char t = buf[a]; buf[a] = buf[b]; buf[b] = t;
    }
    buf[i] = '\0';
}

int main() {
    gpio_init();
    uart_init();

    uart_send_string("VSDSquadron Mini | Firmware v1.0\r\n");

    int count = 0;
    while(1) {
        char buf[16];
        itoa_simple(count++, buf);
        uart_send_string("Count: ");
        uart_send_string(buf);
        uart_send_string("\r\n");
        gpio_toggle();
        for (volatile int i = 0; i < 500000; i++);
    }

    return 0;
}
