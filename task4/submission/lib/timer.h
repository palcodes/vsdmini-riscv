#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief Configures TIM2 to generate a 1 ms update interrupt.
 *
 * Enables RCC clock for TIM2, sets prescaler and auto-reload register
 * for a 1 ms period at system clock, enables the update interrupt (UIE)
 * in TIM2->DMAINTENR, registers the IRQ in the PFIC, and starts the counter.
 *
 * @return void
 *
 * @note Must be called before delay_ms() or get_tick(). Call once at startup.
 */
void timer_init(void);

/**
 * @brief Blocking delay using the TIM2 millisecond tick.
 *
 * Records get_tick() at entry and spins until the elapsed count reaches @p ms.
 * Other ISRs (including TIM2 itself) continue to fire during the wait.
 *
 * @param ms  Delay duration in milliseconds.
 * @return    void
 *
 * @note Blocking — do not call from an ISR context.
 *       Requires timer_init() to have been called first.
 *       Rollover-safe for durations well under ~49 days.
 */
void delay_ms(uint32_t ms);

/**
 * @brief Returns milliseconds elapsed since timer_init() was called.
 *
 * Reads a volatile uint32_t counter incremented by the TIM2 update ISR.
 * No critical section is required for a 32-bit aligned read on RV32EC.
 *
 * @return  Millisecond tick count as uint32_t.
 *
 * @note Rolls over after ~49.7 days. Not a concern for this application.
 */
uint32_t get_tick(void);

#endif
