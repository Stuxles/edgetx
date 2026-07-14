/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "edgetx.h"
#include "board.h"
#include "hal.h"
#include "hal/gpio.h"
#include "stm32_gpio.h"
#if !defined(SIMU)
  #include "os/task.h"
#endif
#include <string.h>

#define RTOS_API_VERSION                0x103
#define TELEMETRY_BUFFER_SIZE           128
#define CROSSFIRE_FIFO_SIZE             256
#define CROSSFIRE_CHANNELS_COUNT        16

template<typename T, int SIZE>
struct FifoBuf {
  volatile uint32_t head;
  volatile uint32_t tail;
  T buffer[SIZE];
};

struct CrossfireSharedData {
  uint32_t rtosApiVersion;
  uint32_t *trampoline;
  uint8_t taskFlag[10];
  volatile uint32_t crsfFlag;
  uint32_t reserved1[16];
  FifoBuf<uint8_t, CROSSFIRE_FIFO_SIZE> crsf_tx; // from XF to OpenTX/EdgeTX
  FifoBuf<uint8_t, CROSSFIRE_FIFO_SIZE> crsf_rx; // from OpenTX/EdgeTX to XF
  uint32_t reserved2[64];
  int16_t sticks[4];
  uint8_t stick_state:5;
  uint8_t gim_select:2;
  uint8_t mixer_schedule:1;
  int32_t channels[CROSSFIRE_CHANNELS_COUNT];
};

typedef struct CrossfireSharedData CrossfireSharedData_t;
#define crossfireSharedData (*((CrossfireSharedData_t*)SHARED_MEMORY_ADDRESS))

extern void trampolineInit(void);

#if !defined(SIMU)
#define CROSSFIRE_STACK_SIZE 1024
#if defined(FREE_RTOS)
  #define CROSSFIRE_TASK_PRIO (tskIDLE_PRIORITY + 5)
#else
  #define CROSSFIRE_TASK_PRIO (5)
#endif

static task_handle_t crossfireTaskId;
static TASK_DEFINE_STACK(crossfireStack, CROSSFIRE_STACK_SIZE);
#endif

void crsfInit(void)
{
#if !defined(SIMU)
  memset((void*)SHARED_MEMORY_ADDRESS, 0, sizeof(CrossfireSharedData));
  crossfireSharedData.rtosApiVersion = RTOS_API_VERSION;
  trampolineInit();

  // Turn on module power
  EXTERNAL_MODULE_PWR_ON();
#endif
}

#if !defined(SIMU)
void tbsCrossfireTaskStart(void)
{
  if (*(uint32_t *)CROSSFIRE_TASK_ADDRESS != 0xFFFFFFFF) {
    task_create(&crossfireTaskId, (task_func_t)CROSSFIRE_TASK_ADDRESS, "crossfire", crossfireStack, CROSSFIRE_STACK_SIZE, CROSSFIRE_TASK_PRIO);
  }
}
#endif

void updateIntCrossfireChannels(void)
{
#if !defined(SIMU)
  for (uint8_t i = 0; i < CROSSFIRE_CHANNELS_COUNT; ++i) {
    crossfireSharedData.channels[i] = channelOutputs[i];
  }
#endif
}

uint16_t getTbsStickValue(uint8_t index)
{
#if !defined(SIMU)
  if (index < 4) {
    int32_t val = crossfireSharedData.sticks[index] + 1024;
    if (val < 0) val = 0;
    if (val > 2047) val = 2047;
    return (uint16_t)val;
  }
#endif
  return 1024;
}

void crsfSharedFifoHandler(void)
{
  // Handle bidirectional FIFO data exchange between EdgeTX and co-resident Crossfire task
}

void crsfSetModelID(void)
{
  // Send model selection subcmd via shared FIFO
}

void crsfGetModelID(void)
{
  // Request current model ID via shared FIFO
}

uint32_t readBackupReg(uint8_t index)
{
#if !defined(SIMU) && defined(BKPSRAM_BASE)
  return *(__IO uint32_t *) (BKPSRAM_BASE + index * 4);
#else
  return 0;
#endif
}

void writeBackupReg(uint8_t index, uint32_t data)
{
#if !defined(SIMU) && defined(BKPSRAM_BASE)
  *(__IO uint32_t *) (BKPSRAM_BASE + index * 4) = data;
#endif
}

uint8_t bkregGetStatusFlag(uint32_t flag)
{
  return (readBackupReg(8) & (1UL << flag)) ? 1 : 0;
}

void bkregSetStatusFlag(uint32_t flag)
{
  writeBackupReg(8, readBackupReg(8) | (1UL << flag));
}

void bkregClrStatusFlag(uint32_t flag)
{
  writeBackupReg(8, readBackupReg(8) & ~(1UL << flag));
}

void crossfirePowerOff(void)
{
#if !defined(SIMU)
  EXTERNAL_MODULE_PWR_OFF();
#endif
}

void crossfireTurnOffRf(bool ask)
{
  crossfireSharedData.crsfFlag |= (1UL << 4); // CRSF_FLAG_RF_OFF
}

void crossfireTurnOnRf(void)
{
  crossfireSharedData.crsfFlag &= ~(1UL << 4);
}

bool isCrossfireRfOn(void)
{
  return !(crossfireSharedData.crsfFlag & (1UL << 4));
}
