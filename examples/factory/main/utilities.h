#pragma once

#define BOARD_SCL  (40)
#define BOARD_SDA  (39)


#define TOUCH_SCL  (BOARD_SCL)
#define TOUCH_SDA  (BOARD_SDA)
#define TOUCH_INT  (3)
#define TOUCH_RST  (9)

//////////
#define BOARD_SPI_MISO    (8)
#define BOARD_SPI_MOSI    (17)
#define BOARD_SPI_SCLK    (18)



#define SD_MISO    (BOARD_SPI_MISO)
#define SD_MOSI    (BOARD_SPI_MOSI)
#define SD_SCLK    (BOARD_SPI_SCLK)
#define SD_CS      (16)

#define LORA_MISO (BOARD_SPI_MISO)
#define LORA_MOSI (BOARD_SPI_MOSI)
#define LORA_SCLK (BOARD_SPI_SCLK)
#define LORA_CS   (46)
#define LORA_IRQ  (3)
#define LORA_RST  (43)
#define LORA_BUSY (44)



#define RTC_INT 7
#define RT_SCL  (BOARD_SCL)
#define RT_SDA  (BOARD_SDA)

#define BL_EN (40)

#define BATT_PIN (4)

#define BOOT_BTN (0)
#define KEY_BTN  (48)


