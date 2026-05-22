all: flash

TARGET:=app/main
TARGET_MCU?=CH32V003

ADDITIONAL_C_FILES=library/timer.c library/gpio.c library/uart.c

include ch32fun/ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
