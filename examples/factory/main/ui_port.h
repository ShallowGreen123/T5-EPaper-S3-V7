#pragma once

#include "lvgl.h"

LV_IMG_DECLARE(img_battery)
LV_IMG_DECLARE(img_clock)
LV_IMG_DECLARE(img_lora)
LV_IMG_DECLARE(img_refresh)
LV_IMG_DECLARE(img_sd_card)
LV_IMG_DECLARE(img_setting)
LV_IMG_DECLARE(img_shutdown)
LV_IMG_DECLARE(img_test)
LV_IMG_DECLARE(img_wifi)

LV_FONT_DECLARE(Font_Mono_Bold_20);
LV_FONT_DECLARE(Font_Mono_Bold_25);
LV_FONT_DECLARE(Font_Mono_Bold_30)
LV_FONT_DECLARE(Font_Mono_Bold_90)
LV_FONT_DECLARE(Font_Geist_Light_20)
LV_FONT_DECLARE(Font_Geist_Bold_20)

void ui_full_refresh(void);
void ui_full_clean(void);

// clock
void ui_clock_get_time(uint8_t *h, uint8_t *m, uint8_t *s);
void ui_clock_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week);

// lora
#define LORA_MODE_SEND 0
#define LORA_MODE_RECV 1

void ui_lora_set_mode(int mode);
void ui_lora_transmit(const char *str);
bool ui_lora_recv(String *str);

// sd
void ui_sd_read(void);

// test
bool ui_test_lora_init(void);
bool ui_test_sd_init(void);

// wifi
bool ui_wifi_get_status(void);
void ui_wifi_set_status(bool statue);

String ui_wifi_get_ip(void);
const char *ui_wifi_get_ssid(void);
const char *ui_wifi_get_pwd(void);















