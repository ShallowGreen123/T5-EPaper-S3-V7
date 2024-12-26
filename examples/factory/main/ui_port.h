#pragma once

#include "lvgl.h"

LV_IMG_DECLARE(img_battery)
LV_IMG_DECLARE(img_clock)
LV_IMG_DECLARE(img_lora)
LV_IMG_DECLARE(img_refresh)
LV_IMG_DECLARE(img_sd_card)
LV_IMG_DECLARE(img_setting)
LV_IMG_DECLARE(img_shutdown)
LV_IMG_DECLARE(img_start)
LV_IMG_DECLARE(img_test)
LV_IMG_DECLARE(img_wifi)

LV_FONT_DECLARE(Font_Mono_Bold_20);
LV_FONT_DECLARE(Font_Mono_Bold_25);
LV_FONT_DECLARE(Font_Mono_Bold_30)
LV_FONT_DECLARE(Font_Mono_Bold_90)
LV_FONT_DECLARE(Font_Geist_Light_20)
LV_FONT_DECLARE(Font_Geist_Bold_20)

void ui_refresh_set_mode(int mode);
void ui_full_refresh(void);
void ui_full_clean(void);
void ui_set_rotation(lv_disp_rot_t rot);

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

// setting
void ui_setting_set_backlight(int bl);
const char *ui_setting_get_backlight(int *ret_bl);
void ui_setting_set_refresh_speed(int bl);
const char *ui_setting_get_refresh_speed(int *ret_bl);

// test
const char *ui_test_get_gps(int *ret_n);
const char *ui_test_get_lora(int *ret_n);
const char *ui_test_get_sd(int *ret_n);
const char *ui_test_get_rtc(int *ret_n);
const char *ui_test_get_touch(int *ret_n);
const char *ui_test_get_BQ25896(int *ret_n);
const char *ui_test_get_BQ27220(int *ret_n);

// wifi
bool ui_wifi_get_status(void);
void ui_wifi_set_status(bool statue);

String ui_wifi_get_ip(void);
const char *ui_wifi_get_ssid(void);
const char *ui_wifi_get_pwd(void);

// battery

int battery_get_capacity(void);
/* 25896 */
void battery_chg_encharge(void);
void battery_chg_discharge(void);
bool battery_25896_is_vaild(void);
bool battery_25896_is_chr(void);
void battery_25896_refr(void);
const char * battery_25896_get_CHG_ST(void);
const char * battery_25896_get_VBUS_ST(void);
const char * battery_25896_get_NTC_ST(void);
float battery_25896_get_VBUS(void);
float battery_25896_get_VSYS(void);
float battery_25896_get_VBAT(void);
float battery_25896_get_targ_VOLT(void);
float battery_25896_get_CHG_CURR(void);
float battery_25896_get_PREC_CURR(void);
/* 27220 */
bool battery_27220_is_vaild(void);
bool battery_27220_is_chr(void);
float battery_27220_get_VOLT(void);
float battery_27220_get_VOLT_CHG(void);
float battery_27220_get_CURR_ARG(void);
float battery_27220_get_CURR_INS(void);
float battery_27220_get_CURR_STD(void);
float battery_27220_get_CURR_CHG(void);
float battery_27220_get_TEMP(void);
float battery_27220_get_BATT_CAP(void);
float battery_27220_get_BATT_CAP_FULL(void);

// shutdown
void ui_shutdown(void);

void ui_sleep(void);









