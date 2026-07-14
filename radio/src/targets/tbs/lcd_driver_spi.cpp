/*
 * Copyright (C) OpenTX / EdgeTX
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "hal/gpio.h"
#include "stm32_gpio.h"
#include "stm32_spi.h"
#include "stm32_dma.h"
#include "board.h"
#include "hal/abnormal_reboot.h"
#include "timers_driver.h"
#include "lcd.h"

#if !defined(BOOT)
  #include "edgetx.h"
#endif
#include "hal.h"

#if defined(RADIO_TANGO) || defined(RADIO_TANGO2)
#define WAIT_FOR_DMA_END()            do { } while (lcd_busy)

#define LCD_CS_HIGH()                 gpio_set(LCD_SPI_CS_GPIO)
#define LCD_CS_LOW()                  gpio_clear(LCD_SPI_CS_GPIO)

#define LCD_RST_HIGH()                gpio_set(LCD_SPI_RST_GPIO)
#define LCD_RST_LOW()                 gpio_clear(LCD_SPI_RST_GPIO)

#define LCD_DC_HIGH()                 gpio_set(LCD_SPI_A0_GPIO)
#define LCD_DC_LOW()                  gpio_clear(LCD_SPI_A0_GPIO)

#define SPI_TIMEOUT                   10000000UL

volatile bool lcd_busy = false;
bool lcd_on = false;

static void spiWrite(uint8_t byte)
{
  LCD_DC_LOW();
  LCD_CS_LOW();

  uint32_t timeout = 0;
  while ((LCD_SPI->SR & SPI_SR_TXE) == 0 && timeout++ < SPI_TIMEOUT) {
    // Wait
  }

  if (timeout >= SPI_TIMEOUT) {
    LCD_CS_HIGH();
    return;
  }

  timeout = 0;
  (void)LCD_SPI->DR; // Clear receive
  LCD_SPI->DR = byte;

  while ((LCD_SPI->SR & SPI_SR_RXNE) == 0 && timeout++ < SPI_TIMEOUT) {
    // Wait
  }

  LCD_CS_HIGH();
}

void spiWriteCommand(uint8_t command)
{
  spiWrite(command);
}

void spiWriteArg(uint8_t arg)
{
  spiWrite(arg);
}

void spiWriteCommandWithArg(uint8_t command, uint8_t arg)
{
  spiWriteCommand(command);
  spiWriteArg(arg);
}

void lcdHardwareInit()
{
  stm32_spi_enable_clock(LCD_SPI);
  gpio_init_af(LCD_MOSI_GPIO, LCD_GPIO_AF, GPIO_PIN_SPEED_HIGH);
  gpio_init_af(LCD_CLK_GPIO, LCD_GPIO_AF, GPIO_PIN_SPEED_HIGH);

  LCD_SPI->CR1 = 0;
  LCD_SPI->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPOL | SPI_CR1_CPHA | LCD_SPI_PRESCALER;
  LCD_SPI->CR2 = 0;
  LCD_SPI->CR1 |= SPI_CR1_MSTR;
  LCD_SPI->CR1 |= SPI_CR1_SPE;

  gpio_init(LCD_SPI_CS_GPIO, GPIO_OUT, GPIO_PIN_SPEED_HIGH);
  LCD_CS_HIGH();

  gpio_init(LCD_SPI_RST_GPIO, GPIO_OUT, GPIO_PIN_SPEED_HIGH);
  gpio_init(LCD_SPI_A0_GPIO, GPIO_OUT, GPIO_PIN_SPEED_HIGH);

  stm32_dma_enable_clock(LCD_DMA);
  LCD_DMA_Stream->CR &= ~DMA_SxCR_EN;
  LCD_DMA->HIFCR = LCD_DMA_FLAGS;
  LCD_DMA_Stream->CR = DMA_SxCR_PL_0 | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | LCD_SPI_DMA_Channel;
  LCD_DMA_Stream->PAR = (uint32_t)&LCD_SPI->DR;

  NVIC_SetPriority(LCD_DMA_Stream_IRQn, 2);
  NVIC_EnableIRQ(LCD_DMA_Stream_IRQn);
}

void lcdInit()
{
  lcdHardwareInit();
  LCD_RST_LOW();
  delay_ms(100);
  LCD_RST_HIGH();
  delay_ms(100);

  // SSD1327 OLED initialization sequence
  spiWriteCommand(0xAE); // Display OFF
  spiWriteCommandWithArg(0x15, 0x00); // Set Column Address
  spiWriteArg(0x3F);
  spiWriteCommandWithArg(0x75, 0x00); // Set Row Address
  spiWriteArg(0x7F);
  spiWriteCommandWithArg(0x81, 0x80); // Set Contrast
  spiWriteCommandWithArg(0xA0, 0x51); // Set Re-map
  spiWriteCommandWithArg(0xA1, 0x00); // Set Display Start Line
  spiWriteCommandWithArg(0xA2, 0x00); // Set Display Offset
  spiWriteCommandWithArg(0xA4, 0xA4); // Normal Display
  spiWriteCommandWithArg(0xA8, 0x3F); // Set Multiplex Ratio
  spiWriteCommandWithArg(0xB1, 0xF1); // Set Phase Length
  spiWriteCommandWithArg(0xB3, 0x00); // Set Front Clock Divider
  spiWriteCommandWithArg(0xAB, 0x01); // Set VDD Internal
  spiWriteCommandWithArg(0xB6, 0x0F); // Set Second Pre-charge Period
  spiWriteCommandWithArg(0xBE, 0x0F); // Set VCOMH Voltage
  spiWriteCommandWithArg(0xBC, 0x08); // Set Pre-charge Voltage
  spiWriteCommandWithArg(0xD5, 0x62); // Set Function Selection B
  spiWriteCommandWithArg(0xFD, 0x12); // Unlock Command
  spiWriteCommand(0xAF); // Display ON

  lcd_on = true;
}

void lcdRefresh(bool wait)
{
  uint8_t * p = displayBuf;
  if (!lcd_on) return;

  LCD_CS_LOW();
  LCD_DC_LOW(); // command write

  spiWriteCommand(0x75);
  spiWriteArg(0);
  spiWriteArg(LCD_H - 1);

  spiWriteCommand(0x15);
  spiWriteArg(0);
  spiWriteArg((LCD_W / 2) - 1);

  LCD_DC_HIGH(); // data write
  LCD_CS_LOW();

  lcd_busy = true;
  LCD_SPI_DMA_Stream->CR &= ~DMA_SxCR_EN;
  LCD_SPI_DMA_Stream->M0AR = (uint32_t)p;
  LCD_SPI_DMA_Stream->NDTR = 16 * 32 * 12;
  LCD_SPI_DMA_Stream->CR |= DMA_SxCR_EN | DMA_SxCR_TCIE;
  LCD_SPI->CR2 |= SPI_CR2_TXDMAEN;

  if (wait) {
    WAIT_FOR_DMA_END();
  }
}

extern "C" void LCD_SPI_DMA_Stream_IRQHandler()
{
  LCD_SPI_DMA_Stream->CR &= ~DMA_SxCR_TCIE;
  LCD_SPI->CR2 &= ~SPI_CR2_TXDMAEN;
  LCD_SPI_DMA_Stream->CR &= ~DMA_SxCR_EN;

  while (LCD_SPI->SR & SPI_SR_BSY) {
    // Wait for SPI completion
  }
  LCD_CS_HIGH();
  lcd_busy = false;
}

/*
  Proper method for turning off LCD module. It must be used,
  otherwise we might damage LCD crystals in the long run!
*/
void lcdOff()
{
  WAIT_FOR_DMA_END();
  spiWriteCommand(0xAE); // LCD sleep
  delay_ms(3); // Wait for caps to drain
  lcd_on = false;
}

void lcdSetRefVolt(uint8_t val)
{
  spiWriteCommandWithArg(0x81, val);
}

#elif defined(RADIO_MAMBO)

#define WAIT_FOR_DMA_END()            do { } while (lcd_busy)
#define LCD_CS_HIGH()                 LCD_SPI_CS_GPIO->BSRRL = LCD_SPI_CS_GPIO_PIN
#define LCD_CS_LOW()                  LCD_SPI_CS_GPIO->BSRRH = LCD_SPI_CS_GPIO_PIN
#define LCD_DC_HIGH()                 LCD_SPI_A0_GPIO->BSRRL = LCD_SPI_A0_GPIO_PIN
#define LCD_DC_LOW()                  LCD_SPI_A0_GPIO->BSRRH = LCD_SPI_A0_GPIO_PIN
#define LCD_RST_HIGH()                LCD_SPI_RST_GPIO->BSRRL = LCD_SPI_RST_GPIO_PIN
#define LCD_RST_LOW()                 LCD_SPI_RST_GPIO->BSRRH = LCD_SPI_RST_GPIO_PIN

volatile bool lcd_busy = false;
bool lcd_on = false;
bool lcdInitFinished = false;

static void spiWrite(uint8_t byte)
{
  LCD_DC_LOW();
  LCD_CS_LOW();
  while ((LCD_SPI->SR & SPI_SR_TXE) == 0) {}
  (void)LCD_SPI->DR;
  LCD_SPI->DR = byte;
  while ((LCD_SPI->SR & SPI_SR_RXNE) == 0) {}
  LCD_CS_HIGH();
}

void spiWriteCommand(uint8_t command) { spiWrite(command); }
void spiWriteArg(uint8_t arg) { spiWrite(arg); }

void lcdHardwareInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  LCD_SPI->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_MSTR | SPI_CR1_SPE;
  LCD_CS_HIGH();

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_CS_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(LCD_SPI_CS_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_RST_GPIO_PIN;
  GPIO_Init(LCD_SPI_RST_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_A0_GPIO_PIN;
  GPIO_Init(LCD_SPI_A0_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_GPIO_PIN_SCK | LCD_SPI_GPIO_PIN_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(LCD_SPI_GPIO, &GPIO_InitStructure);

  GPIO_PinAFConfig(LCD_SPI_GPIO, LCD_SPI_GPIO_PinSource_MOSI, LCD_SPI_GPIO_AF);
  GPIO_PinAFConfig(LCD_SPI_GPIO, LCD_SPI_GPIO_PinSource_SCK, LCD_SPI_GPIO_AF);
}

void lcdInit()
{
  lcdHardwareInit();
  LCD_RST_LOW();
  delay_ms(150);
  LCD_RST_HIGH();
  delay_ms(20);

  spiWriteCommand(0xAE); // Display OFF
  spiWriteCommand(0xA2); // LCD bias 1/9
  spiWriteCommand(0xA0); // ADC Select Normal
  spiWriteCommand(0xC8); // Common output mode reverse
  spiWriteCommand(0x25); // Regulation ratio
  spiWriteCommand(0x81); // Electronic volume mode set
  spiWriteCommand(0x20); // Electronic volume register set
  spiWriteCommand(0x2F); // Power control set
  spiWriteCommand(0xAF); // Display ON

  lcd_on = true;
  lcdInitFinished = true;
}

void lcdRefresh(bool wait)
{
  if (!lcd_on) return;
  for (uint8_t page = 0; page < (LCD_H / 8); page++) {
    spiWriteCommand(0xB0 | page);
    spiWriteCommand(0x10);
    spiWriteCommand(0x00);
    LCD_DC_HIGH();
    LCD_CS_LOW();
    for (uint8_t col = 0; col < LCD_W; col++) {
      while ((LCD_SPI->SR & SPI_SR_TXE) == 0) {}
      (void)LCD_SPI->DR;
      LCD_SPI->DR = displayBuf[page * LCD_W + col];
      while ((LCD_SPI->SR & SPI_SR_RXNE) == 0) {}
    }
    LCD_CS_HIGH();
  }
}

/*
  Proper method for turning off LCD module. It must be used,
  otherwise we might damage LCD crystals in the long run!
*/
void lcdOff()
{
  WAIT_FOR_DMA_END();
  spiWriteCommand(0xAE); // LCD sleep
  delay_ms(3); // Wait for caps to drain
  lcd_on = false;
}

void lcdSetRefVolt(uint8_t val)
{
  spiWriteCommand(0x81);
  spiWriteCommand(val & 0x3F);
}

#endif

void lcdSetInvert(bool invert)
{
  spiWriteCommand(invert ? 0xA7 : 0xA6);
}


