/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "edgetx.h"
#include "board.h"
#include "hal.h"
#include <string.h>
#include <stdlib.h>

#define USB_HID_FIFO_SIZE                       128
#define HID_AGENT_IN_PACKET                     64
#define HID_AGENT_OUT_PACKET                    64
#define LIBCRSF_UART_SYNC                       0xC8
#define LIBCRSF_BF_LINK_STATISTICS              0x14

template<typename T, int SIZE>
struct UsbAgentFifo {
  volatile uint32_t head;
  volatile uint32_t tail;
  T buffer[SIZE];

  bool push(T val) {
    uint32_t next = (head + 1) % SIZE;
    if (next == tail) return false;
    buffer[head] = val;
    head = next;
    return true;
  }

  bool pop(T &val) {
    if (head == tail) return false;
    val = buffer[tail];
    tail = (tail + 1) % SIZE;
    return true;
  }

  uint32_t size() const {
    return (head + SIZE - tail) % SIZE;
  }

  bool hasSpace(uint32_t count) const {
    return (SIZE - 1 - size()) >= count;
  }
};

static UsbAgentFifo<uint8_t, USB_HID_FIFO_SIZE> * hidTxFifo = nullptr;

void usbAgentInit(void)
{
  hidTxFifo = nullptr;
}

void usbTX(void)
{
  static bool isbusy = false;
  if (!isbusy) {
    isbusy = true;
    uint8_t sendData[HID_AGENT_IN_PACKET];
    memset(sendData, 0, HID_AGENT_IN_PACKET);
    if (hidTxFifo != nullptr && hidTxFifo->size() > 0) {
      for (uint8_t i = 0; i < HID_AGENT_IN_PACKET; i++) {
        if (!hidTxFifo->pop(sendData[i])) {
          break;
        }
      }
      // Send HID report if USB agent active
    }
    isbusy = false;
  }
}

void crsfToUsbHid(uint8_t *pArr)
{
  if (hidTxFifo == nullptr) {
    hidTxFifo = (UsbAgentFifo<uint8_t, USB_HID_FIFO_SIZE>*)malloc(sizeof(UsbAgentFifo<uint8_t, USB_HID_FIFO_SIZE>));
    if (hidTxFifo != nullptr) {
      memset(hidTxFifo, 0, sizeof(UsbAgentFifo<uint8_t, USB_HID_FIFO_SIZE>));
    }
  }

  if (pArr[2] != LIBCRSF_BF_LINK_STATISTICS) {
    uint8_t len = pArr[1] + 2;
    if (hidTxFifo != nullptr && hidTxFifo->hasSpace(len)) {
      for (uint8_t i = 0; i < len && i < HID_AGENT_IN_PACKET; i++) {
        hidTxFifo->push(pArr[i]);
      }
    }
  }
  usbTX();
}

void agentHandler(void)
{
  // Check received HID reports and route to co-resident Crossfire task via shared memory
  usbTX();
}
