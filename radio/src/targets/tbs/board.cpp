/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "hal/gpio.h"
#include "stm32_gpio.h"
#include "hal/usb_driver.h"
#if !defined(BOOT)
#include "lcd.h"
#include "edgetx.h"
#endif
#include "board.h"
#include "hal.h"
#include <string.h>

uint32_t pcbRevision = 0;
static bool skipCharging = false;

static uint32_t trampoline[8] = {0};

// Co-resident Crossfire RTOS Trampoline stubs / wrappers
extern "C" uint32_t CoWaitForSingleFlag(uint8_t id, uint32_t timeout) {
  // Bridge wait for single flag to FreeRTOS/EdgeTX event notification if needed
  return 0;
}

extern "C" void CoClearFlag(uint8_t id) {
  // Bridge clear flag to FreeRTOS/EdgeTX event notification if needed
}

void trampolineInit(void) {
  memset(trampoline, 0, sizeof(trampoline));
  trampoline[3] = (uint32_t)(&CoWaitForSingleFlag); // RTOS_WAIT_FLAG_TRAMPOILINE
  trampoline[4] = (uint32_t)(&CoClearFlag);         // RTOS_CLEAR_FLAG_TRAMPOILINE
}



void init2MhzTimer() {
#if !defined(SIMU)
  // Timer initialization for 2MHz reference if needed by external module / pulses
#endif
}

void init5msTimer() {
#if !defined(SIMU)
  // 5ms system timer initialisation
#endif
}



static void runPwrOffCharging(void) {
  // When plugged in via USB while off, run minimal charging loop and check power button
#if !defined(SIMU)
  while (1) {
    if (usbPlugged()) {
      // Handle charging state
    }
    // If power button pressed long enough, boot main radio system
    if (pwrPressed()) {
      break;
    }
  }
#endif
}

void boardInit() {
#if !defined(SIMU)
  // Clear software/watchdog reset flags
  if (RCC->CSR & (RCC_CSR_WDGRSTF | RCC_CSR_SFTRSTF)) {
    RCC->CSR |= RCC_CSR_RMVF;
  }

  // Detect PCB revision if applicable
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  pcbRevision = PCBREV_Tango2_V2;
#elif defined(RADIO_MAMBO)
  pcbRevision = PCBREV_Mambo_V1;
#endif

  trampolineInit();
  crsfInit();

#if defined(ROTARY_ENCODER_NAVIGATION)
  rotaryEncoderInit();
#endif

  if (skipCharging) {
    runPwrOffCharging();
  }
#endif
}

void boardOff() {
#if !defined(SIMU)
#if !defined(BOOT)
  BACKLIGHT_DISABLE();
  lcdOff();
#endif
  SysTick->CTRL = 0; // turn off systick

  if (usbPlugged()) {
    NVIC_SystemReset();
  }

  pwrOff();
  __disable_irq();

  while (1) {
    // Standby sleep loop
    PWR->CR |= PWR_CR_CWUF;
    PWR->CR |= PWR_CR_PDDS;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __WFE();
  }
#endif
}
