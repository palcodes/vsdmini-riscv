#include "timer.h"
#include "../ch32fun/ch32fun/ch32fun.h"
#include "../ch32fun/ch32fun/ch32v003hw.h"

static volatile uint32_t ticks = 0;

void TIM2_IRQHandler(void) __attribute__((interrupt));

void timer_init(void) {

  RCC->APB1PCENR |= RCC_TIM2EN;

  TIM2->PSC = 48 - 1;
  TIM2->ATRLR = 1000 - 1;

  TIM2->SWEVGR = TIM_UG;

  TIM2->INTFR = 0;

  TIM2->DMAINTENR |= TIM_UIE;

  NVIC_EnableIRQ(TIM2_IRQn);

  TIM2->CTLR1 |= TIM_CEN;
}

void delay_ms(uint32_t ms) {
  uint32_t start = ticks;

  while ((ticks - start) < ms)
    ;
}

uint32_t get_tick(void) { return ticks; }

void TIM2_IRQHandler(void) {

  if (TIM2->INTFR & TIM_FLAG_Update) {

    ticks++;

    TIM2->INTFR = ~TIM_FLAG_Update;
  }
}
