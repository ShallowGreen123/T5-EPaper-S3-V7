#include "ui.h"
#include "Arduino.h"
#include "main.h"
#include "utilities.h"

#define T5_EPER_S3_SF_VER "v1.0 24.12.03"

int ui_setting_backlight = 0;

//************************************[ Other fun ]******************************************

void ui_full_refresh(void)
{
    disp_full_refresh();
}

void ui_full_clean(void)
{
    disp_full_clean();
}

//************************************[ screen 1 ]****************************************** clock
void ui_clock_get_time(uint8_t *h, uint8_t *m, uint8_t *s)
{
    // *h = timeinfo.tm_hour;
    // *m = timeinfo.tm_min;
    // *s = timeinfo.tm_sec;

    RTC_DateTime datetime = rtc.getDateTime();
    *h = datetime.hour;
    *m = datetime.minute;
    *s = datetime.second;

    printf("h=%d, m=%d, s=%d\n", *h, *m, *s);

    // static int test_m = 19;
    // *h = 10;
    // *m = test_m++;
    // *s = 0;
}

void ui_clock_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week)
{
    // *year = timeinfo.tm_year % 100;
    // *month = timeinfo.tm_mon+1;
    // *day = timeinfo.tm_mday;
    // *week = timeinfo.tm_wday;

    RTC_DateTime datetime = rtc.getDateTime();
    *year = datetime.year % 100;
    *month = datetime.month;
    *day = datetime.day;
    *week = datetime.week;

    printf("y=%d, m=%d, d=%d, w=%d\n", *year, *month, *day, *week);

    // static int test_data = 18;
    // *year = 24;
    // *month = 12;
    // *day = test_data++;
    // *week = 2;
}
//************************************[ screen 2 ]****************************************** lora
void ui_lora_set_mode(int mode)
{
    // lora_set_mode(mode);
}

void ui_lora_transmit(const char *str)
{
    // lora_transmit(str);
}

bool ui_lora_recv(String *str)
{
    // return lora_receive(str);
    return false;
}
//************************************[ screen 3 ]****************************************** sd_card
void ui_sd_read(void)
{

}
//************************************[ screen 4 ]****************************************** setting
#if 1
void ui_setting_set_backlight(int bl)
{
    bl++;
    bl &= 0x03;

    switch (bl)
    {
        case 0: analogWrite(BL_EN, 0); break;
        case 1: analogWrite(BL_EN, 50); break;
        case 2: analogWrite(BL_EN, 100); break;
        case 3: analogWrite(BL_EN, 230); break;
        default:
            break;
    }
    ui_setting_backlight = bl;
}

const char *ui_setting_get_backlight(int *ret_bl)
{
    const char *ret = NULL;
    switch (ui_setting_backlight)
    {  
        case 0: ret = "OFF"; break;
        case 1: ret = "Low"; break;
        case 2: ret = "Medium"; break;
        case 3: ret = "High"; break;   
        default:
            break;
    }

    if(ret_bl) *ret_bl = ui_setting_backlight;

    return ret;
}

void ui_setting_set_refresh_speed(int bl)
{
    switch (refresh_mode)
    {
        case REFRESH_MODE_FAST:   refresh_mode = REFRESH_MODE_NORMAL;   break;
        case REFRESH_MODE_NORMAL: refresh_mode = REFRESH_MODE_NEAT;     break;
        case REFRESH_MODE_NEAT:   refresh_mode = REFRESH_MODE_FAST;     break;
        default:
            break;
    }
}

const char *ui_setting_get_refresh_speed(int *ret_bl)
{
    const char *ret = NULL;
    switch (refresh_mode)
    {  
        case REFRESH_MODE_FAST:   ret = "Fast";   break;
        case REFRESH_MODE_NORMAL: ret = "Normal"; break;
        case REFRESH_MODE_NEAT:   ret = "Neat";   break;
        default:
            break;
    }

    if(ret_bl) *ret_bl = refresh_mode;

    return ret;
}
#endif
//************************************[ screen 5 ]****************************************** test
bool ui_test_lora_init(void)
{
    return false;
}

bool ui_test_sd_init(void)
{
    return false;
}

//************************************[ screen 6 ]****************************************** wifi
bool ui_wifi_get_status(void)
{
    return false;
}
void ui_wifi_set_status(bool statue)
{

}

String ui_wifi_get_ip(void)
{
    // return WiFi.localIP().toString();
    return "WIFI not connected";
}
const char *ui_wifi_get_ssid(void)
{
    // return wifi_ssid;
    return "WIFI not connected";
}
const char *ui_wifi_get_pwd(void)
{
    // return wifi_password;
    return "WIFI not connected";
}
//************************************[ screen 7 ]****************************************** battery
/* 25896 */

/* 27220 */

//
//************************************[ screen 8 ]****************************************** battery

void ui_shutdown(void)
{

}

//************************************[ home btn ]******************************************
