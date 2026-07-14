/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "hal/gpio.h"
#include "stm32_gpio.h"
#include "stm32_timer.h"
#include "board.h"

void backlightInit()
{
#if defined(BACKLIGHT_GPIO) && !defined(SIMU) && !defined(BOOT)
  gpio_init_af(BACKLIGHT_GPIO, BACKLIGHT_TIMER_AF, GPIO_PIN_SPEED_LOW);
  stm32_timer_enable_clock(BACKLIGHT_TIMER);
  BACKLIGHT_TIMER->ARR = 100;
  BACKLIGHT_TIMER->PSC = BACKLIGHT_TIMER_FREQ / 50000 - 1; // 500Hz
  BACKLIGHT_TIMER->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM
  BACKLIGHT_TIMER->CCER = TIM_CCER_CC1E;
  BACKLIGHT_TIMER->CCR1 = 100;
  BACKLIGHT_TIMER->EGR = 0;
  BACKLIGHT_TIMER->CR1 = TIM_CR1_CEN;
#endif
}

void backlightEnable(uint8_t level)
{
#if defined(BACKLIGHT_GPIO) && !defined(SIMU) && !defined(BOOT)
  BACKLIGHT_TIMER->CCR1 = level;
#endif
}

void backlightDisable()
{
#if defined(BACKLIGHT_GPIO) && !defined(SIMU) && !defined(BOOT)
  BACKLIGHT_TIMER->CCR1 = 0;
#endif
}

void backlightFullOn()
{
  backlightEnable(100);
}
