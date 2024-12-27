#include "ui.h"
#include "Arduino.h"
#include "main.h"
#include "utilities.h"
#include "epdiy.h"

#define T5_EPER_S3_SF_VER "v1.0 24.12.03"

int ui_setting_backlight = 3;  // 0 - 3
int epd_vcom_default = 1560;

//************************************[ Other fun ]******************************************

void ui_indev_touch_en(void)
{
    indev_touch_en();
}

void ui_indev_touch_dis(void)
{
    indev_touch_dis();
}

void ui_refresh_set_mode(int mode)
{
    disp_refresh_set_mode(mode);
}

void ui_full_refresh(void)
{
    disp_full_refresh();
}

void ui_full_clean(void)
{
    disp_full_clean();
}

void ui_set_rotation(lv_disp_rot_t rot)
{
    // EPD_ROT_LANDSCAPE = 0,
    // EPD_ROT_PORTRAIT = 1,
    // EPD_ROT_INVERTED_LANDSCAPE = 2,
    // EPD_ROT_INVERTED_PORTRAIT = 3,
    switch (rot)
    {
        case LV_DISP_ROT_NONE: epd_set_rotation(EPD_ROT_INVERTED_PORTRAIT);  break;
        case LV_DISP_ROT_90:   epd_set_rotation(EPD_ROT_INVERTED_LANDSCAPE); break;
        case LV_DISP_ROT_180:  epd_set_rotation(EPD_ROT_PORTRAIT);           break;
        case LV_DISP_ROT_270:  epd_set_rotation(EPD_ROT_LANDSCAPE);          break;
        default:
            break;
    }
    // LV_DISP_ROT_NONE = 0,
    // LV_DISP_ROT_90,
    // LV_DISP_ROT_180,
    // LV_DISP_ROT_270
    lv_disp_set_rotation(lv_disp_get_default(), rot);
}

//************************************[ screen 1 ]****************************************** clock
void ui_clock_get_time(uint8_t *h, uint8_t *m, uint8_t *s)
{
    // *h = timeinfo.tm_hour;
    // *m = timeinfo.tm_min;
    // *s = timeinfo.tm_sec;

    if(peri_buf[E_PERI_RTC] == true)
    {
        RTC_DateTime datetime = rtc.getDateTime();
        *h = datetime.hour;
        *m = datetime.minute;
        *s = datetime.second;

    } else
    {
        static int test_m = 19;
        *h = 10;
        *m = test_m++;
        *s = 0;
    }

    printf("h=%d, m=%d, s=%d\n", *h, *m, *s);
}

void ui_clock_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week)
{
    // *year = timeinfo.tm_year % 100;
    // *month = timeinfo.tm_mon+1;
    // *day = timeinfo.tm_mday;
    // *week = timeinfo.tm_wday;

    if(peri_buf[E_PERI_RTC] == true)
    {
        RTC_DateTime datetime = rtc.getDateTime();
        *year = datetime.year % 100;
        *month = datetime.month;
        *day = datetime.day;
        *week = datetime.week;
    } else {
        static int test_data = 18;
        *year = 24;
        *month = 12;
        *day = test_data++;
        *week = 2;
    }
    printf("y=%d, m=%d, d=%d, w=%d\n", *year, *month, *day, *week);
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

int ui_setting_get_vcom(void)
{
    return epd_vcom_default;
}

void ui_setting_set_vcom(int v)
{
    v = v > 5000 ? 5000 : v;
    v = v < 200 ? 200 : v;
    epd_vcom_default = v;
    epd_set_vcom(v); // TPS651851 VCOM output range 0-5.1v  step:10mV
}

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
const char *ui_test_get_gps(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_GPS];
    return ((peri_buf[E_PERI_GPS] ==  true) ? "PASS" : "---");
}
const char *ui_test_get_lora(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_LORA];
    return ((peri_buf[E_PERI_LORA] ==  true) ? "PASS" : "---");
}
const char *ui_test_get_sd(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_SD_CARD];
    return ((peri_buf[E_PERI_SD_CARD] ==  true) ? "PASS" : "---");
}
const char *ui_test_get_rtc(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_RTC];
    return ((peri_buf[E_PERI_RTC] ==  true) ? "PASS" : "---");
}
const char *ui_test_get_touch(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_TOUCH];
    return ((peri_buf[E_PERI_TOUCH] ==  true) ? "PASS" : "---");
}
const char *ui_test_get_BQ25896(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_BQ25896];
    return ((peri_buf[E_PERI_BQ25896] ==  true) ? "PASS" : "---");
}
const char *ui_test_get_BQ27220(int *ret_n)
{
    if(ret_n) *ret_n = peri_buf[E_PERI_BQ27220];
    return ((peri_buf[E_PERI_BQ27220] ==  true) ? "PASS" : "---");
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
#if 1

int battery_get_capacity(void)
{
    static int bat = 50;
    // return lv_rand(0, 100);

    bat += 5;
    if(bat >= 100) {
        bat = 0;
    }

    return bat;
}

/* 25896 */
void battery_chg_encharge(void)
{
    PPM.enableCharge();
}

void battery_chg_discharge(void)
{
    PPM.disableCharge();
}

bool battery_25896_is_vaild(void)
{
    return peri_buf[E_PERI_BQ25896];
    // return 1;
}

bool battery_25896_is_chr(void)
{
    if(PPM.isCharging() == false) {
        return false;
    } else {
        return true;
    }
    return 0;
}

void battery_25896_refr(void)
{
}

const char * battery_25896_get_CHG_ST(void)
{
    return PPM.getChargeStatusString();
    // return 0;
}
const char * battery_25896_get_VBUS_ST(void) 
{
    return PPM.getBusStatusString();
    return 0;
}
const char * battery_25896_get_NTC_ST(void)
{
    return PPM.getNTCStatusString();
    // return 0;
}
float battery_25896_get_VBUS(void)
{
    return (PPM.getVbusVoltage() *1.0 / 1000.0 );
    // return 0;
}
float battery_25896_get_VSYS(void) 
{
    return (PPM.getSystemVoltage() * 1.0 / 1000.0);
    // return 0;
}
float battery_25896_get_VBAT(void)
{
    return (PPM.getBattVoltage() * 1.0 / 1000.0);
    // return 0;
}
float battery_25896_get_targ_VOLT(void)
{
    return (PPM.getChargeTargetVoltage() * 1.0 / 1000.0);
    // return 0;
}
float battery_25896_get_CHG_CURR(void)
{
    return (PPM.getChargeCurrent());
    // return 0;
}
float battery_25896_get_PREC_CURR(void)
{
    return (PPM.getPrechargeCurr());
    // return 0;
}

/* 27220 */
bool battery_27220_is_vaild(void)
{
    return peri_buf[E_PERI_BQ27220];
    // return 0;
}

bool battery_27220_is_chr(void)
{
    return bq27220.getIsCharging();
    // return 0;
}

float battery_27220_get_VOLT(void)
{
    return bq27220.getVolt(VOLT);
    // return 0;
}
float battery_27220_get_VOLT_CHG(void)
{
    return bq27220.getVolt(VOLT_CHARGING);
    // return 0;
}
float battery_27220_get_CURR_ARG(void)
{
    return bq27220.getCurr(CURR_AVERAGE);
    // return 0;
}
float battery_27220_get_CURR_INS(void)
{
    return bq27220.getCurr(CURR_INSTANT);
    // return 0;
}
float battery_27220_get_CURR_STD(void)
{
    return bq27220.getCurr(CURR_STANDBY);
    // return 0;
}
float battery_27220_get_CURR_CHG(void)
{
    return bq27220.getCurr(CURR_CHARGING);
    // return 0;
}
float battery_27220_get_TEMP(void)
{
    return (float)(bq27220.getTemp() / 10 - 273); // 摄氏度
    // return 0;
}
float battery_27220_get_BATT_CAP(void)
{
    return bq27220.getRemainCap();
    // return 0;
}
float battery_27220_get_BATT_CAP_FULL(void)
{
    return bq27220.getFullChargeCap();
    // return 0;
}

#endif
//************************************[ screen 8 ]****************************************** shutdown

void ui_shutdown(void)
{
    PPM.shutdown();
}

void ui_sleep(void)
{
    touch.sleep();
    radio.sleep();

    digitalWrite(TOUCH_RST, LOW); 
    digitalWrite(LORA_RST, LOW); 

    pinMode(9, OUTPUT);
    digitalWrite(9, LOW); 
    
    gpio_hold_en((gpio_num_t)TOUCH_RST);
    gpio_hold_en((gpio_num_t)LORA_RST);
    gpio_hold_en((gpio_num_t)9);
    gpio_deep_sleep_hold_en();

    analogWrite(BL_EN, 0);

    epd_poweroff();

    esp_sleep_enable_ext0_wakeup((gpio_num_t)BOOT_BTN, 0);
    // esp_sleep_enable_ext1_wakeup((1UL << KEY_BTN), ESP_EXT1_WAKEUP_ANY_LOW); 
    esp_deep_sleep_start();
}

//************************************[ home btn ]******************************************
