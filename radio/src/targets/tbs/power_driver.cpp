/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "hal/gpio.h"
#include "stm32_gpio.h"
#include "board.h"
#include "hal/abnormal_reboot.h"

void pwrInit()
{
#if defined(PWR_SWITCH_GPIO)
  gpio_init(PWR_SWITCH_GPIO, GPIO_IN_PD, GPIO_PIN_SPEED_LOW);
#endif
}

void pwrOn()
{
#if defined(PWR_ON_GPIO)
  gpio_init(PWR_ON_GPIO, GPIO_OUT, GPIO_PIN_SPEED_LOW);
  gpio_set(PWR_ON_GPIO);
#endif
}

void pwrOff()
{
#if defined(PWR_ON_GPIO)
  gpio_clear(PWR_ON_GPIO);
#endif
}

bool pwrPressed()
{
#if defined(PWR_SWITCH_GPIO)
  return gpio_read(PWR_SWITCH_GPIO);
#else
  return true;
#endif
}

bool pwrOffPressed()
{
#if defined(PWR_BUTTON_PRESS)
  return pwrPressed();
#elif defined(PWR_SWITCH_GPIO)
  return !pwrPressed();
#else
  return false;
#endif
}

void pwrResetHandler()
{
  if (WAS_RESET_BY_WATCHDOG_OR_SOFTWARE()) {
    pwrOn();
  }
}

#if defined(BOOT)
void* _pwr_init_hook[] __INIT_HOOK = {
  (void*)pwrResetHandler,    
};
#endif
