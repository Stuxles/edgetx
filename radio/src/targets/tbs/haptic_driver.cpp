/*
 * Copyright (C) EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "hal/gpio.h"
#include "stm32_gpio.h"
#include "board.h"
#include "hal.h"

void hapticInit(void)
{
  gpio_init(HAPTIC_GPIO, GPIO_OUT, GPIO_PIN_SPEED_LOW);
  gpio_clear(HAPTIC_GPIO);
}

void hapticOn(uint32_t pwmPercent)
{
  if (pwmPercent > 0) {
    gpio_set(HAPTIC_GPIO);
  } else {
    gpio_clear(HAPTIC_GPIO);
  }
}

void hapticOff(void)
{
  gpio_clear(HAPTIC_GPIO);
}

void hapticDone(void)
{
  hapticOff();
}
