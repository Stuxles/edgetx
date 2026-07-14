/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#pragma once

#include <inttypes.h>
#include "definitions.h"
#include "edgetx_constants.h"
#include "board_common.h"
#include "hal.h"
#include "hal/watchdog_driver.h"
#include "hal/serial_port.h"

#if defined(ROTARY_ENCODER_NAVIGATION)
// Rotary Encoder driver
void rotaryEncoderInit();
void rotaryEncoderCheck();
#endif

#if !defined(LUA_EXPORT_GENERATION) && !defined(SIMU)
#include "stm32f4xx.h"
#endif

#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
#define MY_DEVICE_NAME                  "Tango II"
#elif defined(RADIO_MAMBO)
#define MY_DEVICE_NAME                  "Mambo"
#endif

#define LCD_CONTRAST_MIN                2
#define LCD_CONTRAST_MAX                254
#define LCD_CONTRAST_DEFAULT            32

#define SLAVE_MODE()                    (g_model.trainerData.mode == TRAINER_MODE_SLAVE)

#define FLASHSIZE                       0xC0000
#define FLASH_PAGESIZE                  256
#define BOOTLOADER_SIZE                 0xC000
#define FIRMWARE_ADDRESS                0x08000000
#define CROSSFIRE_TASK_ADDRESS          0x080C0020
#define SHARED_MEMORY_ADDRESS           0x10000000

#define FIRMWARE_LEN(fsize)             (fsize - BOOTLOADER_SIZE)
#define FIRMWARE_MAX_LEN                (FLASHSIZE - BOOTLOADER_SIZE)
#define APP_START_ADDRESS               (uint32_t)(FIRMWARE_ADDRESS + BOOTLOADER_SIZE)

#define LUA_MEM_MAX                     (0)    // max allowed memory usage for complete Lua  (in bytes), 0 means unlimited

#define PERI1_FREQUENCY                 42000000
#define PERI2_FREQUENCY                 84000000

#define TIMER_MULT_APB1                 2
#define TIMER_MULT_APB2                 2

#define BOOTLOADER_KEYS                 0x42

extern uint16_t sessionTimer;

#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
static const uint8_t switchPosition[][2] = {{0,0}, {0,1}, {1,1}, {1,0}, {1,2}, {0,2}};
#elif defined(RADIO_MAMBO)
static const uint8_t switchPosition[][2] = {{0,1}, {0,0}, {1,1}, {1,0}, {0,2}, {1,2}};
#endif

// Board driver
void boardInit();
void boardOff();
void lcdInit();
void lcdOff();
void lcdRefresh(bool wait = true);
#define lcdRefreshWait()
void lcdSetContrast(bool useDefault = false);
void lcdSetRefVolt(uint8_t val);

void pwrInit();
uint32_t pwrCheck();
void pwrOn();
void pwrOff();
bool pwrPressed();
bool pwrOffPressed();
void pwrResetHandler();
#define pwrForcePressed()               false

void backlightInit();
void backlightEnable(uint8_t level);
void backlightDisable();
void backlightFullOn();
#define BACKLIGHT_ENABLE()              backlightEnable(100)
#define BACKLIGHT_DISABLE()             backlightDisable()
#define BACKLIGHT_LEVEL_MIN             0
#define BACKLIGHT_LEVEL_MAX             100
#define BACKLIGHT_FORCED_ON             101

// Timers driver
void init2MhzTimer();
void init5msTimer();

// PCBREV driver
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
enum {
  PCBREV_Tango2_Unknown = 0,
  PCBREV_Tango2_V1,
  PCBREV_Tango2_V2, // no external power module required
  PCBREV_Tango2_V3,
};
#elif defined(RADIO_MAMBO)
enum {
  PCBREV_Mambo_Unknown = 0,
  PCBREV_Mambo_V1,
};
#endif
extern uint32_t pcbRevision;

// Pulses / External module driver
#define INTERNAL_MODULE_ON()
#define INTERNAL_MODULE_OFF()

// Crossfire / USB Agent
void crsfInit();
void usbAgentInit();
uint16_t getTbsStickValue(uint8_t index);

// Haptic driver
void hapticInit(void);
void hapticOn(uint32_t pwmPercent);
void hapticOff(void);
void hapticDone(void);

// Battery warning limits
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  // 1S LiPo / LiIon
  #define BATTERY_WARN                  35 // 3.5V
  #define BATTERY_MIN                   34 // 3.4V
  #define BATTERY_MAX                   42 // 4.2V
#elif defined(RADIO_MAMBO)
  // 2S LiPo / LiIon
  #define BATTERY_WARN                  66 // 6.6V
  #define BATTERY_MIN                   67 // 6.7V
  #define BATTERY_MAX                   83 // 8.3V
#endif

#define BATTERY_DIVIDER                 26214
#define VOLTAGE_DROP                    20

// Status LEDs & LCD utilities
void ledRed();
void ledGreen();
void ledBlue();
void ledOff();
void lcdSetInvert(bool invert);

#define DEBUG_BAUDRATE                  115200
#define LUA_DEFAULT_BAUDRATE            115200

const etx_serial_port_t* auxSerialGetPort(int port_nr);

