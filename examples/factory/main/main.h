#pragma once
// include 
#include <Arduino.h>
// #include "epd_driver.h"
#include "utilities.h"
// #include "firasans.h"
#include "esp_adc_cal.h"
#include <Wire.h>
#include "TouchDrvGT911.hpp"
#include <SensorPCF8563.hpp>
#include <WiFi.h>
#include <esp_sntp.h>
#include <RadioLib.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include "bq27220.h"
#define XPOWERS_CHIP_BQ25896
#include <XPowersLib.h>
#include "board/pca9555.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

// io_extend
extern "C" {
    void io_extend_lora_gps_power_on(bool en);
    uint8_t read_io(int io);
    void set_config(i2c_port_t port, uint8_t config_value, int high_port);
    bool button_read(void);
}

// peripheral
// |        RTC (PCF8563)         |
// | :--------------------------: |
// |        BQ25896 (MPU)         |
// |     BQ27220 (Coulometer)     |
// |           SD Card            |
// |        GT911 (Tocuh)         |
// |     PCA9535 (IO extend)      |
// | TPS651851 (Ink Screen Power) |
// |        Lora (SX1262)         |
// |             GPS              |
enum {
    E_PERI_INK_POWER = 0,
    E_PERI_BQ25896,
    E_PERI_BQ27220,
    E_PERI_RTC,
    E_PERI_TOUCH,
    E_PERI_LORA,
    E_PERI_SD_CARD,
    E_PERI_GPS,
    E_PERI_MAX,
};

extern bool peri_buf[E_PERI_MAX];

// task
extern TaskHandle_t gps_handle;
// extern TaskHandle_t lora_handle;

// lora
extern SX1262 radio;

extern BQ27220 bq27220;

// bq25896
extern XPowersPPM PPM;

// display refresh mode
#define REFRESH_MODE_FAST   0
#define REFRESH_MODE_NORMAL 1
#define REFRESH_MODE_NEAT   2
extern int refresh_mode;

void disp_full_refresh(void);
void disp_full_clean(void);
void disp_refresh_set_mode(int mode);

void indev_touch_en();
void indev_touch_dis();

// Touch
extern TouchDrvGT911 touch;

// RTC
extern SensorPCF8563 rtc;


