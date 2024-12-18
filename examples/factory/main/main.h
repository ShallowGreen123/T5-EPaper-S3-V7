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
// #include <RadioLib.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
// #include "bq27220.h"
#include <XPowersLib.h>


void disp_full_refresh(void);
void disp_full_clean(void);

