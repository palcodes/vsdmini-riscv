#include "../ch32fun/ch32fun/ch32fun.h"
#include "../lib/gpio.h"
#include "../lib/timer.h"
#include "../lib/uart.h"

#define LED_PIN 4
#define CMD_BUF_LEN 32

static void print_help(void) {
  uart_send_string("\r\nCommands:\r\n");
  uart_send_string("  help          - show this menu\r\n");
  uart_send_string("  led on        - turn LED on\r\n");
  uart_send_string("  led off       - turn LED off\r\n");
  uart_send_string("  blink <ms>    - blink LED every <ms> milliseconds\r\n");
  uart_send_string("  read <pin>    - read GPIOD pin state\r\n");
  uart_send_string("  stop          - stop blinking\r\n\r\n");
}

static uint32_t parse_number(const char *s) {
  uint32_t n = 0;
  while (*s >= '0' && *s <= '9')
    n = n * 10 + (*s++ - '0');
  return n;
}

static int str_eq(const char *a, const char *b) {
  while (*a && *b)
    if (*a++ != *b++)
      return 0;
  return *a == *b;
}

static int str_starts(const char *s, const char *prefix) {
  while (*prefix)
    if (*s++ != *prefix++)
      return 0;
  return 1;
}

int main(void) {
  SystemInit();
  gpio_init(LED_PIN);
  timer_init();
  uart_init();

  uart_send_string("\r\nVSDSquadron Mini | UART CLI v1.0\r\n");
  print_help();
  uart_send_string("> ");

  char buf[CMD_BUF_LEN];
  uint8_t idx = 0;
  buf[0] = '\0';

  uint8_t blinking = 0;
  uint32_t blink_ms = 500;
  uint32_t last_toggle = 0;

  while (1) {
    if (blinking) {
      uint32_t now = get_tick();
      if ((now - last_toggle) >= blink_ms) {
        gpio_toggle(LED_PIN);
        last_toggle = now;
      }
    }

    if (!uart_data_ready())
      continue;

    char c = uart_read_char();

    if (c == '\b' || c == 0x7F) {
      if (idx > 0) {
        idx--;
        uart_send_string("\b \b");
      }
    } else if (c == '\n') {
      uart_send_string("\r\n");
      buf[idx] = '\0';
      idx = 0;

      if (buf[0] == '\0') {
        uart_send_string("> ");
        continue;
      }

      if (str_eq(buf, "help")) {
        print_help();
      } else if (str_eq(buf, "led on")) {
        blinking = 0;
        gpio_set(LED_PIN);
        uart_send_string("LED turned on\r\n");
      } else if (str_eq(buf, "led off")) {
        blinking = 0;
        gpio_clear(LED_PIN);
        uart_send_string("LED turned off\r\n");
      } else if (str_starts(buf, "blink ")) {
        uint32_t ms = parse_number(buf + 6);
        if (ms == 0) {
          uart_send_string("Error: invalid interval\r\n");
        } else {
          blink_ms = ms;
          blinking = 1;
          last_toggle = get_tick();
          uart_send_string("Blinking every ");
          uart_send_number(ms);
          uart_send_string("ms\r\n");
        }
      } else if (str_starts(buf, "read ")) {
        uint32_t pin = parse_number(buf + 5);
        if (pin > 7) {
          uart_send_string("Error: pin must be 0-7\r\n");
        } else {
          uart_send_string("PD");
          uart_send_number(pin);
          uart_send_string(" = ");
          uart_send_number(gpio_read((uint8_t)pin));
          uart_send_string("\r\n");
        }
      } else if (str_eq(buf, "stop")) {
        blinking = 0;
        gpio_clear(LED_PIN);
        uart_send_string("Stopped\r\n");
      }
      // Following block is a test command to see how the LED is driven on Port
      else if (str_eq(buf, "test")) {
        gpio_set(LED_PIN);
        uart_send_string("After set:   OUTDR bit = ");
        uart_send_number((GPIOD->OUTDR >> LED_PIN) & 1);
        uart_send_string("\r\n");

        gpio_clear(LED_PIN);
        uart_send_string("After clear: OUTDR bit = ");
        uart_send_number((GPIOD->OUTDR >> LED_PIN) & 1);
        uart_send_string("\r\n");
      } else {
        uart_send_string("Unknown command. Type 'help'\r\n");
      }

      uart_send_string("> ");
    } else if (idx < CMD_BUF_LEN - 1) {
      buf[idx++] = c;
      uart_send_char(c);
    }
  }
}
