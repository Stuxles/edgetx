/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "hal/key_driver.h"
#include "hal.h"
#include "stm32_hal_ll.h"
#include "stm32_gpio_driver.h"
#include "keys.h"
#include "hal/adc_driver.h"
#include "stm32_keys.inc"

#if defined(ROTARY_ENCODER_NAVIGATION)
uint32_t rotencPositionValue = 0;
#endif

#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
#if !defined(BOOT)
uint8_t g_trimState = 0;
#endif
#elif defined(RADIO_MAMBO)
#define POT_MULTIPOS_SWITCH_ADC_TOLERANCE   128
#define POT_3POS_SWITCH_POS1_ADC  0
#define POT_3POS_SWITCH_POS2_ADC  1024
#define POT_3POS_SWITCH_POS3_ADC  2048

const uint16_t trimAdcValue[] = {4095/2, 4095/3, 4095/4, 4095/5, 4095/6, 4095/7, 4095/8, 4095/9};
const int trimAdcBoundary[] = {(4095 + trimAdcValue[0]) >> 1,            (trimAdcValue[0] + trimAdcValue[1]) >> 1,
                               (trimAdcValue[1] + trimAdcValue[2]) >> 1, (trimAdcValue[2] + trimAdcValue[3]) >> 1,
                               (trimAdcValue[3] + trimAdcValue[4]) >> 1, (trimAdcValue[4] + trimAdcValue[5]) >> 1,
                               (trimAdcValue[5] + trimAdcValue[6]) >> 1, (trimAdcValue[6] + trimAdcValue[7]) >> 1,
                               (trimAdcValue[7] + 0) >> 1};
#define IS_TRIM_PRESSED_STATE(x)  (trimAdcBoundary[x] > trimValue && trimValue > trimAdcBoundary[x + 1])
#endif

uint32_t readKeys()
{
  return _read_keys();
}

uint32_t readTrims()
{
  uint32_t result = 0;

#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  result = _read_trims();
#if !defined(BOOT)
  result |= g_trimState;
  g_trimState = 0;
#endif
#elif defined(RADIO_MAMBO)
  uint16_t trimValue = getAnalogValue(SWITCH_TRIM);
  for (uint8_t x = 0; x < TRM_LAST - TRM_BASE + 1; x++) {
    if (IS_TRIM_PRESSED_STATE(x)) {
      if (x == 4)
        result |= 1 << 5;
      else if (x == 5)
        result |= 1 << 4;
      else
        result |= 1 << x;
      break;
    }
  }
#else
  result = _read_trims();
#endif

  return result;
}

void keysInit()
{
  _init_keys();
  _init_trims();

#if defined(ROTARY_ENCODER_NAVIGATION)
  rotencPositionValue = ROTARY_ENCODER_POSITION;
#endif
}
