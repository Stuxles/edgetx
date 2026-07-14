/*
 * Copyright (C) EdgeTX
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef _USB_DESCRIPTOR_H_
#define _USB_DESCRIPTOR_H_

#if defined(RADIO_MAMBO)
  #define USB_NAME                     "MAMBO"
  #define USB_MANUFACTURER             'T', 'B', 'S', ' ', ' ', ' ', ' ', ' '  /* 8 bytes */
  #define USB_PRODUCT                  'M', 'A', 'M', 'B', 'O', ' ', ' ', ' '  /* 8 Bytes */
#else
  #define USB_NAME                     "TANGO2"
  #define USB_MANUFACTURER             'T', 'B', 'S', ' ', ' ', ' ', ' ', ' '  /* 8 bytes */
  #define USB_PRODUCT                  'T', 'A', 'N', 'G', 'O', '2', ' ', ' '  /* 8 Bytes */
#endif

#endif // _USB_DESCRIPTOR_H_
