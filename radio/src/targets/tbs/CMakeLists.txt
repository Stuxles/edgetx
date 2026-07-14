option(INTERNAL_MODULE_CRSF "Support for crossfire internal module" ON)
option(CROSSFIRE_TASK "Crossfire task" YES)
option(ENABLE_ROTARY_INVERSE "Enable rotary inverse" ON)
option(USB_CHARGER "USB Charger Support" ON)

set(PWR_BUTTON "PRESS" CACHE STRING "Pwr button type (PRESS/SWITCH)")
set(STICKS "STANDARD" CACHE STRING "Sticks type (STANDARD/HORUS)")
set(CPU_TYPE STM32F4)
set(CPU_TYPE_FULL STM32F407xE)
set(HSE_VALUE 8000000)
set(SDCARD YES)
set(EEPROM SDCARD)
set(TARGET_DIR tbs)
include_directories(targets/${TARGET_DIR})
set(PPM_LIMITS_SYMETRICAL YES)
set(HAPTIC YES)
set(USB_CHARGER YES)
set(HARDWARE_EXTERNAL_MODULE YES)
set(ROTARY_ENCODER YES)

if(PCB STREQUAL TANGO2)
  set(FLAVOUR tango2)
  set(LCD_DEPTH 4)
  add_definitions(-DRADIO_TANGO -DRADIO_TANGO2 -DPCBTANGO2)
elseif(PCB STREQUAL MAMBO)
  set(FLAVOUR mambo)
  set(LCD_DEPTH 1)
  add_definitions(-DHARDWARE_TRIMS -DRADIO_MAMBO -DPCBMAMBO)
endif()

set(LINKER_SCRIPT targets/tbs/stm32f4_tango2.ld)
set(TARGET_LINKER_DIR stm32f40x)

add_definitions(-DSTM32F40_41xxx -DSTM32F407xx)
add_definitions(-DRADIO_FAMILY_TBS)
add_definitions(-DEEPROM_VARIANT=0x8000)
add_definitions(-DPWR_BUTTON_${PWR_BUTTON})
add_definitions(-DSOFTWARE_VOLUME)
add_definitions(-DBATT_CRITICAL_SHUTDOWN)
add_definitions(-DSD_CONFIG_PROTECTION)
add_definitions(-DMANUFACTURER_TBS)
add_definitions(-DCROSSFIRE_TASK)
add_definitions(-DHARDWARE_INTERNAL_MODULE -DINTERNAL_MODULE_CRSF)
add_definitions(-DESP_SERIAL)
add_definitions(-DENABLE_ROTARY_INVERSE)

set(GUI_DIR 128x64)
set(BITMAPS_DIR 128x64)
set(BITMAPS_TARGET 9x_bitmaps)
set(FONTS_TARGET 9x_fonts_1bit)
set(LCD_DRIVER lcd_driver_spi.cpp)

set(GUI_SRC ${GUI_SRC}
  model_input_edit.cpp
  model_mix_edit.cpp
  model_display.cpp
  radio_diagkeys.cpp
  radio_diaganas.cpp
  view_channels.cpp
  view_telemetry.cpp
  view_about.cpp
  bmp.cpp
)

set(DEFAULT_INTERNAL_MODULE CROSSFIRE CACHE STRING "Default internal module")
set(INTERNAL_MODULES CRSF CACHE STRING "Internal modules")

set(TARGET_SRC_DIR targets/${TARGET_DIR})

set(BOARD_COMMON_SRC
  ${TARGET_SRC_DIR}/board.cpp
  ${TARGET_SRC_DIR}/backlight_driver.cpp
  ${TARGET_SRC_DIR}/${LCD_DRIVER}
  ${TARGET_SRC_DIR}/power_driver.cpp
  ${TARGET_SRC_DIR}/tbs_keys.cpp
  ${TARGET_SRC_DIR}/haptic_driver.cpp
  targets/common/arm/stm32/delays_driver.cpp
  targets/common/arm/stm32/flash_driver.cpp
  targets/common/arm/stm32/rtc_driver.cpp
  targets/common/arm/stm32/watchdog_driver.cpp
  targets/common/arm/stm32/abnormal_reboot.cpp
)

if(ROTARY_ENCODER)
  list(APPEND BOARD_COMMON_SRC
    targets/common/arm/stm32/rotary_encoder_driver.cpp
  )
endif()

# Bootloader board library
add_library(board_bl OBJECT EXCLUDE_FROM_ALL
  ${BOARD_COMMON_SRC}
  ${RADIO_SRC_DIR}/gui/common/stdlcd/boot_menu.cpp
)
set(BOOTLOADER_SRC ${BOOTLOADER_SRC} $<TARGET_OBJECTS:board_bl>)

# Firmware board library
add_library(board OBJECT EXCLUDE_FROM_ALL
  ${BOARD_COMMON_SRC}
  ${TARGET_SRC_DIR}/tbs_crsf.cpp
  ${TARGET_SRC_DIR}/tbs_usb_agent.cpp
  targets/common/arm/stm32/audio_dac_driver.cpp
  targets/common/arm/stm32/heartbeat_driver.cpp
  targets/common/arm/stm32/mixer_scheduler_driver.cpp
  targets/common/arm/stm32/module_timer_driver.cpp
  targets/common/arm/stm32/stm32_pulse_driver.cpp
  targets/common/arm/stm32/stm32_rgbleds.cpp
  targets/common/arm/stm32/stm32_softserial_driver.cpp
  targets/common/arm/stm32/stm32_switch_driver.cpp
  targets/common/arm/stm32/trainer_driver.cpp
)
set(FIRMWARE_SRC ${FIRMWARE_SRC} $<TARGET_OBJECTS:board>)
