/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef _HAL_TBS_H_
#define _HAL_TBS_H_

#include "hal_settings.h"

// Keys
#define KEYS_GPIO_REG_MENU                GPIOD->IDR
#define KEYS_GPIO_PIN_MENU                GPIO_Pin_13  // PD.13
#define KEYS_GPIO_REG_EXIT                GPIOD->IDR
#define KEYS_GPIO_PIN_EXIT                GPIO_Pin_14  // PD.14
#define KEYS_GPIO_REG_PAGE                GPIOD->IDR
#define KEYS_GPIO_PIN_PAGE                GPIO_Pin_12  // PD.12
#define KEYS_GPIO_REG_ENTER               GPIOD->IDR
#define KEYS_GPIO_PIN_ENTER               GPIO_Pin_4   // PD.4

// Rotary Encoder
#define ROTARY_ENCODER_NAVIGATION
#define ROTARY_ENCODER_GPIO               GPIOA
#define ROTARY_ENCODER_GPIO_PIN_A         LL_GPIO_PIN_8    // PA.8
#define ROTARY_ENCODER_GPIO_PIN_B         LL_GPIO_PIN_10   // PA.10
#define ROTARY_ENCODER_POSITION           (((ROTARY_ENCODER_GPIO->IDR >> 9) & 0x02) | ((ROTARY_ENCODER_GPIO->IDR >> 8) & 0x01))
#define ROTARY_ENCODER_EXTI_LINE1         LL_EXTI_LINE_8
#define ROTARY_ENCODER_EXTI_LINE2         LL_EXTI_LINE_10
#define ROTARY_ENCODER_EXTI_PORT          LL_SYSCFG_EXTI_PORTA
#define ROTARY_ENCODER_EXTI_PORT_A        LL_SYSCFG_EXTI_PORTA
#define ROTARY_ENCODER_EXTI_PORT_B        LL_SYSCFG_EXTI_PORTA
#define ROTARY_ENCODER_EXTI_SYS_LINE1     LL_SYSCFG_EXTI_LINE8
#define ROTARY_ENCODER_EXTI_SYS_LINE2     LL_SYSCFG_EXTI_LINE10
#if !defined(USE_EXTI9_5_IRQ)
  #define USE_EXTI9_5_IRQ
  #define EXTI9_5_IRQ_Priority 5
#endif
#if !defined(USE_EXTI15_10_IRQ)
  #define USE_EXTI15_10_IRQ
  #define EXTI15_10_IRQ_Priority 5
#endif
#if defined(ROTARY_ENCODER_NAVIGATION)
  #define ROTARY_ENCODER_RCC_APB2Periph   RCC_APB2Periph_TIM10
  #define ROTARY_ENCODER_TIMER            TIM10
  #define ROTARY_ENCODER_TIMER_IRQn       TIM1_UP_TIM10_IRQn
  #define ROTARY_ENCODER_TIMER_IRQHandler TIM1_UP_TIM10_IRQHandler
#else
  #define ROTARY_ENCODER_RCC_APB1Periph   0
#endif

// Switches
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  #define STORAGE_SWITCH_A
  #define HARDWARE_SWITCH_A
  #define SWITCHES_GPIO_REG_A             GPIOC->IDR
  #define SWITCHES_GPIO_PIN_A             GPIO_Pin_4 // PC.04

  #define STORAGE_SWITCH_B
  #define HARDWARE_SWITCH_B
  #define SWITCHES_GPIO_REG_B_H           GPIOA->IDR
  #define SWITCHES_GPIO_PIN_B_H           GPIO_Pin_5 // PA.05
  #define SWITCHES_GPIO_REG_B_L           GPIOA->IDR
  #define SWITCHES_GPIO_PIN_B_L           GPIO_Pin_6 // PA.06

  #define STORAGE_SWITCH_C
  #define HARDWARE_SWITCH_C
  #define SWITCHES_GPIO_REG_C_H           GPIOE->IDR
  #define SWITCHES_GPIO_PIN_C_H           GPIO_Pin_3 // PE.03
  #define SWITCHES_GPIO_REG_C_L           GPIOE->IDR
  #define SWITCHES_GPIO_PIN_C_L           GPIO_Pin_2 // PE.02

  #define STORAGE_SWITCH_D
  #define HARDWARE_SWITCH_D
  #define SWITCHES_GPIO_REG_D             GPIOE->IDR
  #define SWITCHES_GPIO_PIN_D             GPIO_Pin_0  // PE.00

  #define STORAGE_SWITCH_E
  #define HARDWARE_SWITCH_E
  #define SWITCHES_GPIO_REG_E             GPIOE->IDR
  #define SWITCHES_GPIO_PIN_E             GPIO_Pin_1  // PE.01

  #define STORAGE_SWITCH_F
  #define HARDWARE_SWITCH_F
  #define SWITCHES_GPIO_REG_F             GPIOA->IDR
  #define SWITCHES_GPIO_PIN_F             GPIO_Pin_7 // PA.07

  #define KEYS_RCC_AHB1Periph             (RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG)
  #define KEYS_GPIOA_PINS                 (SWITCHES_GPIO_PIN_F | SWITCHES_GPIO_PIN_B_H | SWITCHES_GPIO_PIN_B_L | ROTARY_ENCODER_GPIO_PIN_A | ROTARY_ENCODER_GPIO_PIN_B)
  #define KEYS_GPIOB_PINS                 (0)
  #define KEYS_GPIOC_PINS                 (SWITCHES_GPIO_PIN_A)
  #define KEYS_GPIOD_PINS                 (KEYS_GPIO_PIN_ENTER | KEYS_GPIO_PIN_MENU | KEYS_GPIO_PIN_PAGE | KEYS_GPIO_PIN_EXIT)
  #define KEYS_GPIOE_PINS                 (SWITCHES_GPIO_PIN_C_H | SWITCHES_GPIO_PIN_C_L | SWITCHES_GPIO_PIN_D | SWITCHES_GPIO_PIN_E)
  #define KEYS_GPIOF_PINS                 (0)
#elif defined(RADIO_MAMBO)
  #define HARDWARE_SWITCH_A
  #define HARDWARE_SWITCH_B
  #define HARDWARE_SWITCH_C
  #define HARDWARE_SWITCH_D
  #define HARDWARE_SWITCH_E
  #define SWITCHES_GPIO_REG_E             GPIOE->IDR
  #define SWITCHES_GPIO_PIN_E             GPIO_Pin_5  // PE.05
  #define HARDWARE_SWITCH_F
  #define SWITCHES_GPIO_REG_F             GPIOE->IDR
  #define SWITCHES_GPIO_PIN_F             GPIO_Pin_4  // PE.04

  #define KEYS_RCC_AHB1Periph             (RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG)
  #define KEYS_GPIOA_PINS                 (ROTARY_ENCODER_GPIO_PIN_A | ROTARY_ENCODER_GPIO_PIN_B)
  #define KEYS_GPIOB_PINS                 (0)
  #define KEYS_GPIOC_PINS                 (0)
  #define KEYS_GPIOD_PINS                 (KEYS_GPIO_PIN_ENTER | KEYS_GPIO_PIN_MENU | KEYS_GPIO_PIN_PAGE | KEYS_GPIO_PIN_EXIT)
  #define KEYS_GPIOE_PINS                 (SWITCHES_GPIO_PIN_E | SWITCHES_GPIO_PIN_F)
  #define KEYS_GPIOF_PINS                 (0)
#endif

// ADC
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  #define ADC_MAIN                        ADC1
  #define ADC_DMA                         DMA2
  #define ADC_DMA_SxCR_CHSEL              0
  #define ADC_DMA_Stream                  DMA2_Stream4
  #define ADC_SET_DMA_FLAGS()             ADC_DMA->HIFCR = (DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4)
  #define ADC_TRANSFER_COMPLETE()         (ADC_DMA->HISR & DMA_HISR_TCIF4)
  #define ADC_SAMPTIME                    2   // sample time = 28 cycles
  #define ADC_RCC_AHB1Periph              (RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2)
  #define ADC_RCC_APB2Periph              RCC_APB2Periph_ADC1
  #define ADC_GPIO_PIN_BATT               GPIO_Pin_1  // PB.01
  #define ADC_GPIOB_PINS                  ADC_GPIO_PIN_BATT
  #define ADC_CHANNEL_BATT                ADC_Channel_9  // ADC1_IN9
  #define ADC_VREF_PREC2                  307
#elif defined(RADIO_MAMBO)
  #define ADC_MAIN                        ADC1
  #define ADC_DMA                         DMA2
  #define ADC_DMA_SxCR_CHSEL              0
  #define ADC_DMA_Stream                  DMA2_Stream4
  #define ADC_SET_DMA_FLAGS()             ADC_DMA->HIFCR = (DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4)
  #define ADC_TRANSFER_COMPLETE()         (ADC_DMA->HISR & DMA_HISR_TCIF4)
  #define ADC_SAMPTIME                    2   // sample time = 28 cycles
  #define ADC_VREF_PREC2                  300

  #define HARDWARE_POT1
  #define HARDWARE_POT2
  #define ADC_RCC_AHB1Periph              (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA2)
  #define ADC_RCC_APB2Periph              RCC_APB2Periph_ADC1

  #define ADC_GPIO_PIN_SWITCH_A           GPIO_Pin_6  // PA.06
  #define ADC_GPIO_PIN_SWITCH_B           GPIO_Pin_7  // PA.07
  #define ADC_GPIO_PIN_SWITCH_C           GPIO_Pin_5  // PA.05
  #define ADC_GPIO_PIN_SWITCH_D           GPIO_Pin_3  // PA.03
  #define ADC_GPIO_PIN_POT1               GPIO_Pin_4  // PC.04
  #define ADC_GPIO_PIN_POT2               GPIO_Pin_2  // PA.02
  #define ADC_GPIO_PIN_TRIM               GPIO_Pin_1  // PC.01

  #define ADC_GPIO_PIN_BATT               GPIO_Pin_1  // PB.01
  #define ADC_GPIOA_PINS                  (ADC_GPIO_PIN_SWITCH_A | ADC_GPIO_PIN_SWITCH_B | ADC_GPIO_PIN_SWITCH_C | ADC_GPIO_PIN_SWITCH_D | ADC_GPIO_PIN_POT2)
  #define ADC_GPIOB_PINS                  (ADC_GPIO_PIN_BATT)
  #define ADC_GPIOC_PINS                  (ADC_GPIO_PIN_POT1 | ADC_GPIO_PIN_TRIM)

  #define ADC_CHANNEL_BATT                ADC_Channel_9   // ADC1_IN9
  #define ADC_CHANNEL_SWITCH_A            ADC_Channel_6   // ADC1_IN6
  #define ADC_CHANNEL_SWITCH_B            ADC_Channel_7   // ADC1_IN7
  #define ADC_CHANNEL_SWITCH_C            ADC_Channel_5   // ADC1_IN5
  #define ADC_CHANNEL_SWITCH_D            ADC_Channel_3   // ADC1_IN3
  #define ADC_CHANNEL_POT1                ADC_Channel_14  // ADC1_IN14
  #define ADC_CHANNEL_POT2                ADC_Channel_2   // ADC1_IN2
  #define ADC_CHANNEL_TRIM                ADC_Channel_11  // ADC1_IN11
#endif

// PWR driver
#define PWR_SWITCH_GPIO                   GPIO_PIN(GPIOB, 14) // PB.14
#define PWR_SWITCH_PIN_ACTIVE_HIGH
#define PWR_ON_GPIO                       GPIO_PIN(GPIOB, 12) // PB.12

// Charger
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  #define CHARGER_STATE_GPIO              GPIOD
  #define CHARGER_STATE_GPIO_PIN          GPIO_Pin_10 // PD.10
  #define CHARGER_FAULT_GPIO              GPIOD
  #define CHARGER_FAULT_GPIO_PIN          GPIO_Pin_11 // PD.11
#elif defined(RADIO_MAMBO)
  #define CHARGER_STATE_GPIO              GPIOD
  #define CHARGER_STATE_GPIO_PIN          GPIO_Pin_11 // PD.11
  #define CHARGER_FAULT_GPIO              0
  #define CHARGER_FAULT_GPIO_PIN          0
#endif

// External Module
#define EXTMODULE_RCC_AHB1Periph          (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_DMA2)
#define EXTMODULE_RCC_APB2Periph          (RCC_APB2Periph_TIM8 | RCC_APB2Periph_USART6)
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  #define EXTMODULE_PWR_GPIO_PIN          GPIO_PIN(GPIOE, 5) // PE.05
#elif defined(RADIO_MAMBO)
  #define EXTMODULE_PWR_GPIO_PIN          GPIO_PIN(GPIOE, 2) // PE.02
#endif
#define EXTERNAL_MODULE_PWR_ON()          gpio_set(EXTMODULE_PWR_GPIO_PIN)
#define EXTERNAL_MODULE_PWR_OFF()         gpio_clear(EXTMODULE_PWR_GPIO_PIN)
#define EXTMODULE_TX_GPIO                 GPIOC
#define EXTMODULE_USART_GPIO              EXTMODULE_TX_GPIO
#define EXTMODULE_TX_GPIO_PIN             GPIO_PIN(GPIOC, 6)  // PC.06
#define EXTMODULE_TX_GPIO_PinSource       GPIO_PinSource6
#define EXTMODULE_RX_GPIO_PIN             GPIO_PIN(GPIOC, 7)  // PC.07
#define EXTMODULE_RX_GPIO_PinSource       GPIO_PinSource7
#define EXTMODULE_TIMER                   TIM8
#define EXTMODULE_TIMER_FREQ              (PERI2_FREQUENCY * TIMER_MULT_APB2)
#define EXTMODULE_TIMER_CC_IRQn           TIM8_CC_IRQn
#define EXTMODULE_TIMER_CC_IRQHandler     TIM8_CC_IRQHandler
#define EXTMODULE_TIMER_TX_GPIO_AF        GPIO_AF_TIM8
#define EXTMODULE_TIMER_DMA_CHANNEL       DMA_Channel_7
#define EXTMODULE_TIMER_DMA_STREAM        DMA2_Stream1
#define EXTMODULE_TIMER_DMA_STREAM_IRQn   DMA2_Stream1_IRQn
#define EXTMODULE_TIMER_DMA_STREAM_IRQHandler DMA2_Stream1_IRQHandler
#define EXTMODULE_TIMER_DMA_FLAG_TC       DMA_IT_TCIF1
#define EXTMODULE_TIMER_OUTPUT_ENABLE     TIM_CCER_CC1E
#define EXTMODULE_TIMER_OUTPUT_POLARITY   TIM_CCER_CC1P
#define EXTMODULE_USART_GPIO_AF           GPIO_AF_USART6
#define EXTMODULE_USART                   USART6
#define EXTMODULE_USART_IRQn              USART6_IRQn
#define EXTMODULE_USART_IRQHandler        USART6_IRQHandler
#define EXTMODULE_USART_TX_DMA_CHANNEL    DMA_Channel_5
#define EXTMODULE_USART_TX_DMA_STREAM     DMA2_Stream6
#define EXTMODULE_USART_RX_DMA_CHANNEL    DMA_Channel_5
#define EXTMODULE_USART_RX_DMA_STREAM     DMA2_Stream1

// Leds
#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
  #define LED_STRIP_GPIO                  GPIO_PIN(GPIOB, 11) // PB.11
  #define LED_STRIP_GPIO_AF               LL_GPIO_AF_1 // TIM2_CH4
  #define LED_STRIP_TIMER                 TIM2
  #define LED_STRIP_TIMER_FREQ            (PERI1_FREQUENCY * TIMER_MULT_APB1)
  #define LED_STRIP_TIMER_CHANNEL         LL_TIM_CHANNEL_CH4
  #define LED_STRIP_TIMER_DMA             DMA1
  #define LED_STRIP_TIMER_DMA_STREAM      LL_DMA_STREAM_6
  #define LED_STRIP_TIMER_DMA_CHANNEL     LL_DMA_CHANNEL_3
  #define LED_STRIP_TIMER_DMA_IRQn        DMA1_Stream6_IRQn
  #define LED_STRIP_TIMER_DMA_IRQHandler  DMA1_Stream6_IRQHandler
  #define LED_STRIP_COUNT                 2
  #define LED_STRIP_LENGTH                2
  #define LED_STRIP_REFRESH_PERIOD        20
#elif defined(RADIO_MAMBO)
  #define STATUS_LED_RCC_AHB1Periph       RCC_AHB1Periph_GPIOF
  #define STATUS_LEDS
  #define STATUS_LEDS_GPIO                GPIOF
  #define STATUS_LED_RED_GPIO_PIN         GPIO_Pin_13
  #define STATUS_LED_GREEN_GPIO_PIN       GPIO_Pin_14
  #define STATUS_LED_BLUE_GPIO_PIN        GPIO_Pin_15
#endif

// Haptic
#define HAPTIC_GPIO                       GPIO_PIN(GPIOB, 0) // PB.00

// USB
#define USB_RCC_AHB1Periph                RCC_AHB1Periph_GPIOA
#define USB_GPIO                          GPIOA
#define USB_GPIO_PIN_VBUS                 GPIO_Pin_9
#define USB_GPIO_PIN_DM                   GPIO_Pin_11
#define USB_GPIO_PIN_DP                   GPIO_Pin_12
#define USB_GPIO_PinSource_DM             GPIO_PinSource11
#define USB_GPIO_PinSource_DP             GPIO_PinSource12

// Telemetry (Internal Crossfire handled by co-resident task)

// LCD SPI
#define LCD_SPI_RCC_AHB1Periph            (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA2)
#define LCD_SPI_PRESCALER                 SPI_CR1_BR_0
#define LCD_MOSI_GPIO                     GPIO_PIN(GPIOA, 7) // PA.7
#define LCD_CLK_GPIO                      GPIO_PIN(GPIOA, 5) // PA.5
#define LCD_GPIO_AF                       LL_GPIO_AF_5
#define LCD_SPI                           SPI1
#define LCD_DMA                           DMA2
#define LCD_DMA_Stream                    DMA2_Stream3
#define LCD_SPI_DMA_Stream                DMA2_Stream3
#define LCD_SPI_DMA_Channel               LL_DMA_CHANNEL_3
#define LCD_DMA_Stream_IRQn               DMA2_Stream3_IRQn
#define LCD_SPI_DMA_Stream_IRQn           DMA2_Stream3_IRQn
#define LCD_DMA_Stream_IRQHandler         DMA2_Stream3_IRQHandler
#define LCD_SPI_DMA_Stream_IRQHandler     DMA2_Stream3_IRQHandler
#define LCD_DMA_FLAGS                     (DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3)
#define LCD_SPI_CS_GPIO                   GPIO_PIN(GPIOA, 4) // PA.4
#define LCD_SPI_RST_GPIO                  GPIO_PIN(GPIOC, 5) // PC.5
#define LCD_SPI_A0_GPIO                   GPIO_PIN(GPIOA, 3) // PA.3

// Backlight
#define BACKLIGHT_TIMER                   TIM4
#define BACKLIGHT_TIMER_AF                LL_GPIO_AF_2
#define BACKLIGHT_GPIO                    GPIO_PIN(GPIOB, 6) // PB.6
#define BACKLIGHT_TIMER_FREQ              (PERI1_FREQUENCY * TIMER_MULT_APB1)

// Audio
#define AUDIO_OUTPUT_GPIO                 GPIO_PIN(GPIOA, 4) // PA.4
#define AUDIO_DMA                         DMA1
#define AUDIO_DMA_Stream                  DMA1_Stream5
#define AUDIO_DMA_Stream_IRQn             DMA1_Stream5_IRQn
#define AUDIO_DMA_Stream_IRQHandler       DMA1_Stream5_IRQHandler
#define AUDIO_TIMER                       TIM6

// SDCard SDIO
#define STORAGE_USE_SDIO
#define SD_PRESENT_GPIO                   GPIO_PIN(GPIOC, 5) // PC.05

#define SD_SDIO_DMA_STREAM                LL_DMA_STREAM_3
#define SD_SDIO_DMA_CHANNEL               LL_DMA_CHANNEL_4
#define SD_SDIO_DMA_IRQn                  DMA2_Stream3_IRQn
#define SD_SDIO_DMA_IRQHANDLER            DMA2_Stream3_IRQHandler
#define SD_SDIO_FIFO_ADDRESS              ((uint32_t)0x40012C80)
#define SD_SDIO_CLK_DIV(fq)               ((48000000 / (fq)) - 2)
#define SD_SDIO_INIT_CLK_DIV              SD_SDIO_CLK_DIV(400000)
#define SD_SDIO_TRANSFER_CLK_DIV          SD_SDIO_CLK_DIV(24000000)

// USB
#define USB_GPIO_VBUS                   GPIO_PIN(GPIOA, 9)  // PA.09
#define USB_GPIO_DM                     GPIO_PIN(GPIOA, 11) // PA.11
#define USB_GPIO_DP                     GPIO_PIN(GPIOA, 12) // PA.12
#define USB_GPIO_AF                     GPIO_AF10

#endif // _HAL_TBS_H_
