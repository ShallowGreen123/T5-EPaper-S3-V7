/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
 *
 * Write an image into a header file using a 3...2...1...0 format per pixel,
 * for 4 bits color (16 colors - well, greys.) MSB first.  At 80 MHz, screen
 * clears execute in 1.075 seconds and images are drawn in 1.531 seconds.
 */

#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_timer.h>
#include <esp_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <epdiy.h>
#include "lvgl.h"
#include "main.h"
#include "ui.h"
#include "ui_port.h"

// Arduino
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <driver/i2c.h>

#define NFC_PRIORITY     (configMAX_PRIORITIES - 1)
#define LORA_PRIORITY    (configMAX_PRIORITIES - 2)
#define WS2812_PRIORITY  (configMAX_PRIORITIES - 3)
#define BATTERY_PRIORITY (configMAX_PRIORITIES - 4)
#define INFARED_PRIORITY (configMAX_PRIORITIES - 5)

TaskHandle_t gps_handle;
TaskHandle_t lora_handle;

// peripheral
bool peri_buf[E_PERI_MAX] = {0};

// lora
SX1262 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);
volatile bool transmittedFlag = false;
int transmissionState = RADIOLIB_ERR_NONE;
int count = 0;// counter to keep track of transmitted packets

// bq25896
XPowersPPM PPM;

BQ27220 bq27220;

// Ink Screen
#define WAVEFORM EPD_BUILTIN_WAVEFORM
#define DEMO_BOARD epd_board_v7
EpdiyHighlevelState hl;
int temperature = 0;
uint8_t* fb = NULL;

// Touch
TouchDrvGT911 touch;

// RTC
SensorPCF8563 rtc;

// LVGL
#define DISP_BUF_SIZE (epd_rotated_display_width() * epd_rotated_display_height())
int refresh_mode = REFRESH_MODE_NORMAL;
uint8_t *decodebuffer = NULL;
lv_timer_t *flush_timer = NULL;
volatile bool disp_flush_enabled = true;
volatile bool indev_touch_enabled = true;
bool disp_refr_is_busy = false;

/*********************************************************************************
 *                                   TASK
 * *******************************************************************************/
void gps_task(void *param)
{
    // SerialGPS.begin(38400, SERIAL_8N1, BOARD_GPS_RXD, BOARD_GPS_TXD);
    while (1)
    {
        while (SerialGPS.available())
        {
            SerialMon.write(SerialGPS.read());
        }
        while (SerialMon.available())
        {
            SerialGPS.write(SerialMon.read());
        }
        delay(500);

        // if (digitalRead(PCA9535_INT) == LOW)
        // {
        //     if(button_read()) {
        //         Serial.printf("Button Press\n");
        //     }else{
        //         Serial.printf("Button Release\n");
        //     }
        // }
        // delay(10);
    }
}

void lora_task(void *param)
{
    while (1)
    {
        if(peri_buf[E_PERI_LORA] == false)
        {
            Serial.println("LoRa Init Error!");
            delay(1000);
        }
        else {
            // check if the previous transmission finished
            if (transmittedFlag)
            {
                // reset flag
                transmittedFlag = false;

                if (transmissionState == RADIOLIB_ERR_NONE)
                {
                    // packet was successfully sent
                    Serial.println(F("transmission finished!"));

                    // NOTE: when using interrupt-driven transmit method,
                    //       it is not possible to automatically measure
                    //       transmission data rate using getDataRate()
                }
                else
                {
                    Serial.print(F("failed, code "));
                    Serial.println(transmissionState);
                }

                // clean up after transmission is finished
                // this will ensure transmitter is disabled,
                // RF switch is powered down etc.
                radio.finishTransmit();

                // wait a second before transmitting again
                delay(1000);

                // send another one
                Serial.print(F("[SX1262] Sending another packet ... "));

                // you can transmit C-string or Arduino string up to
                // 256 characters long
                String str = "Hello World! #" + String(count++);
                transmissionState = radio.startTransmit(str);

                // you can also transmit byte array up to 256 bytes long
                /*
                byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                                    0x89, 0xAB, 0xCD, 0xEF};
                transmissionState = radio.startTransmit(byteArr, 8);
                */
            }
            delay(1);
        }
        
    }
}

/*********************************************************************************
 *                              FUNCTION
 * *******************************************************************************/

void setFlag(void)
{
    // we sent a packet, set the flag
    transmittedFlag = true;
}

static inline void checkError(enum EpdDrawError err) {
    if (err != EPD_DRAW_SUCCESS) {
        ESP_LOGE("demo", "draw error: %X", err);
    }
}

void indev_touch_en()
{
    indev_touch_enabled = true;
}

void indev_touch_dis()
{
    indev_touch_enabled = false;
}

void disp_refresh_set_mode(int mode)
{
    refresh_mode = mode;
}

void disp_full_refresh(void)
{
    epd_hl_set_all_white(&hl);
    epd_poweron();
    checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
    epd_poweroff();
}

void disp_full_clean(void)
{
    int refresh_timer = 12;
    epd_poweron();
    // fill_line_black
    for (int i = 0; i < 10; i++) {
        epd_push_pixels(epd_full_screen(), refresh_timer, 0);
    }
    // fill_line_white
    for(int i = 0; i < 10; i++) {
        epd_push_pixels(epd_full_screen(), refresh_timer, 1);
    }
    // fill_line_noop
    for (int i = 0; i < 2; i++) {
        epd_push_pixels(epd_full_screen(), refresh_timer, 2);
    }
    // epd_clear();
    epd_poweroff();
}

/*********************************************************************************
 *                            STATIC  FUNCTION
 * *******************************************************************************/

static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if(disp_flush_enabled) {
        uint16_t w = lv_area_get_width(area);
        uint16_t h = lv_area_get_height(area);
        lv_color32_t *t32 = (lv_color32_t *)color_p;

        for(int i = 0; i < (w * h) / 2; i++) {

            lv_color8_t ret;
            LV_COLOR_SET_R8(ret, LV_COLOR_GET_R(*t32) >> 5); /*8 - 3  = 5*/
            LV_COLOR_SET_G8(ret, LV_COLOR_GET_G(*t32) >> 5); /*8 - 3  = 5*/
            LV_COLOR_SET_B8(ret, LV_COLOR_GET_B(*t32) >> 6); /*8 - 2  = 6*/
            decodebuffer[i] = ret.full;
            t32++;
        }
        // printf("[disp_flush] x1:%d, y1:%d, w:%d, h:%d\n", area->x1, area->y1, w, h);
    }
    EpdRect rener_area = {
        .x = 0,
        .y = 0,
        .width = epd_rotated_display_width(),
        .height = epd_rotated_display_height(),
    };

    if(refresh_mode == REFRESH_MODE_FAST) 
    {
        // disp_full_refresh();
        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        // checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        checkError(epd_hl_update_area(&hl, MODE_DU, epd_ambient_temperature(), rener_area));
        epd_poweroff();
    } 
    else if(refresh_mode == REFRESH_MODE_NORMAL)
    {
        // disp_full_refresh();
        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        // checkError(epd_hl_update_area(&hl, MODE_DU, epd_ambient_temperature(), rener_area));
        epd_poweroff();
    } 
    else if(refresh_mode == REFRESH_MODE_NEAT)
    {
        disp_full_refresh();
        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        epd_poweroff();
    }
    /* Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(disp);
}

static void flush_timer_cb(lv_timer_t *t)
{
    EpdRect rener_area = {
        .x = 0,
        .y = 0,
        .width = epd_rotated_display_width(),
        .height = epd_rotated_display_height(),
    };

    if(refresh_mode == REFRESH_MODE_FAST) 
    {
        // disp_full_refresh();
        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        // checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        checkError(epd_hl_update_area(&hl, MODE_DU, epd_ambient_temperature(), rener_area));
        epd_poweroff();
    } 
    else if(refresh_mode == REFRESH_MODE_NORMAL)
    {
        // disp_full_refresh();
        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        // checkError(epd_hl_update_area(&hl, MODE_DU, epd_ambient_temperature(), rener_area));
        epd_poweroff();
    } 
    else if(refresh_mode == REFRESH_MODE_NEAT)
    {
        disp_full_refresh();
        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        epd_poweroff();
    }
    // static int cnt = 0;
    // printf("[flush] %d\n", cnt++);
    lv_timer_pause(flush_timer);
}

static void dips_render_start_cb(struct _lv_disp_drv_t * disp_drv)
{
    if(flush_timer == NULL) {
        flush_timer = lv_timer_create(flush_timer_cb, 200, NULL);
        lv_timer_ready(flush_timer);
    } else {
        lv_timer_ready(flush_timer);
        lv_timer_resume(flush_timer);
    }
    // printf("dips_render_start_cb\n");
}

static void my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    static int16_t x=0, y=0;

    if(touch.isPressed() && indev_touch_enabled) {
        // if(touch.getPoint(x, y)){
        if(touch.getPoint(&x, &y, 1)){
            data->state = LV_INDEV_STATE_PRESSED;
            Serial.printf("[input] X:%d Y:%d\n", data->point.x, data->point.y);
        }
    } 
    else{
        data->state = LV_INDEV_STATE_RELEASED;
    }
    data->point.x = x;
    data->point.y = y;
}

static void lv_port_disp_init(void)
{
    lv_init();

    static lv_disp_draw_buf_t draw_buf;

    lv_color_t *lv_disp_buf_1 = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    lv_color_t *lv_disp_buf_2 = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    decodebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), DISP_BUF_SIZE);
    lv_disp_draw_buf_init(&draw_buf, lv_disp_buf_1, lv_disp_buf_2, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = epd_rotated_display_width();
    disp_drv.ver_res = epd_rotated_display_height();
    disp_drv.flush_cb = disp_flush;
    // disp_drv.render_start_cb = dips_render_start_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;                 /*See below.*/
    indev_drv.read_cb = my_input_read;              /*See below.*/
    /*Register the driver in LVGL and save the created input device object*/
    // static lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
    lv_indev_drv_register(&indev_drv);
}

static bool touch_gt911_init(void)
{
    // Touch --- 0x5D
    touch.setPins(TOUCH_RST, TOUCH_INT);
    if (!touch.begin(Wire, GT911_SLAVE_ADDRESS_L, BOARD_SDA, BOARD_SCL))
    {
        while (1) {
            Serial.println("Failed to find GT911 - check your wiring!");
            delay(1000);
        }
    }
    Serial.println("Init GT911 Sensor success!");

    // Set the center button to trigger the callback , Only for specific devices, e.g LilyGo-EPD47 S3 GT911
    touch.setHomeButtonCallback([](void *user_data) {
        Serial.println("Home button pressed!");

        EpdRect rener_area = {
            .x = 0,
            .y = 0,
            .width = epd_rotated_display_width(),
            .height = epd_rotated_display_height(),
        };

        disp_full_clean();
        disp_full_refresh();

        epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
        epd_poweron();
        checkError(epd_hl_update_screen(&hl, MODE_GC16, temperature));
        epd_poweroff();
    }, NULL);

    touch.setInterruptMode(LOW_LEVEL_QUERY);
    return true;
}

static bool rtc_pcf8563_init(void)
{
    pinMode(RTC_IRQ, INPUT_PULLUP);

    if (!rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, RTC_SDA, RTC_SCL)) {
        Serial.println("Failed to find PCF8563 - check your wiring!");
        // while (1) {
        //     delay(1000);
        // }
        return false;
    }

    uint16_t year = 2024;
    uint8_t month = 12;
    uint8_t day = 18;
    uint8_t hour = 10;
    uint8_t minute = 19;
    uint8_t second = 00;

    rtc.setDateTime(year, month, day, hour, minute, second);

    return true;
}

static bool screen_init(void)
{
    epd_init(&DEMO_BOARD, &ED047TC1, EPD_LUT_64K);
    // Set VCOM for boards that allow to set this in software (in mV).
    // This will print an error if unsupported. In this case,
    // set VCOM using the hardware potentiometer and delete this line.
    // epd_set_vcom(1560);
    epd_set_vcom(ui_setting_get_vcom()); // TPS651851 VCOM output range 0-5.1v  step:10mV

    hl = epd_hl_init(WAVEFORM);

    // Default orientation is EPD_ROT_LANDSCAPE
    epd_set_rotation(EPD_ROT_INVERTED_PORTRAIT);

    printf(
        "Dimensions after rotation, width: %d height: %d\n\n", epd_rotated_display_width(),
        epd_rotated_display_height()
    );

    // The display bus settings for V7 may be conservative, you can manually
    // override the bus speed to tune for speed, i.e., if you set the PSRAM speed
    // to 120MHz.
    // epd_set_lcd_pixel_clock_MHz(17);

    heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
    heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);

    // epd_poweron();
    // epd_clear();
    // epd_poweroff();

    disp_full_clean();
    temperature = epd_ambient_temperature();

    printf("current temperature: %d\n", temperature);

    return true;
}

static bool bq25896_init(void)
{
    bool result =  PPM.init(Wire, BOARD_SDA, BOARD_SCL, BQ25896_SLAVE_ADDRESS);
    if (result == false) {
        // while (1) {
        //     Serial.println("PPM is not online...");
        //     delay(1000);
        // }
        return false;
    }
    
    // Set the minimum operating voltage. Below this voltage, the PPM will protect
    PPM.setSysPowerDownVoltage(3300);

    // Set input current limit, default is 500mA
    PPM.setInputCurrentLimit(3250);

    Serial.printf("getInputCurrentLimit: %d mA\n", PPM.getInputCurrentLimit());

    // Disable current limit pin
    PPM.disableCurrentLimitPin();

    // Set the charging target voltage, Range:3840 ~ 4608mV ,step:16 mV
    PPM.setChargeTargetVoltage(4208);

    // Set the precharge current , Range: 64mA ~ 1024mA ,step:64mA
    PPM.setPrechargeCurr(64);

    // The premise is that Limit Pin is disabled, or it will only follow the maximum charging current set by Limi tPin.
    // Set the charging current , Range:0~5056mA ,step:64mA
    PPM.setChargerConstantCurr(1024);

    // Get the set charging current
    PPM.getChargerConstantCurr();
    Serial.printf("getChargerConstantCurr: %d mA\n", PPM.getChargerConstantCurr());


    // To obtain voltage data, the ADC must be enabled first
    PPM.enableMeasure();

    // Turn on charging function
    // If there is no battery connected, do not turn on the charging function
    PPM.enableCharge();

    // Turn off charging function
    // If USB is used as the only power input, it is best to turn off the charging function,
    // otherwise the VSYS power supply will have a sawtooth wave, affecting the discharge output capability.
    // PPM.disableCharge();


    // The OTG function needs to enable OTG, and set the OTG control pin to HIGH
    // After OTG is enabled, if an external power supply is plugged in, OTG will be turned off

    PPM.enableOTG();
    PPM.disableOTG();
    // pinMode(OTG_ENABLE_PIN, OUTPUT);
    // digitalWrite(OTG_ENABLE_PIN, HIGH);

    return result;
}

static bool bq27220_init(void)
{
    return bq27220.init();;
}

static bool lora_sx1262_init(void)
{
    // initialize SX1262 with default settings
    Serial.print(F("[SX1262] Initializing ... "));
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        return false;
        // while (true)
            ;
    }

    // set the function that will be called
    // when packet transmission is finished
    radio.setPacketSentAction(setFlag);

    if (radio.setFrequency(868.0) == RADIOLIB_ERR_INVALID_FREQUENCY)
    {
        Serial.println(F("Selected frequency is invalid for this module!"));
        // while (true)
            ;
    }

    // set bandwidth to 250 kHz
    if (radio.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH)
    {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        // while (true)
            ;
    }

    // set spreading factor to 10
    if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
    {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        // while (true)
            ;
    }

    // set coding rate to 6
    if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE)
    {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        // while (true)
            ;
    }

    // set LoRa sync word to 0xAB
    if (radio.setSyncWord(0xAB) != RADIOLIB_ERR_NONE)
    {
        Serial.println(F("Unable to set sync word!"));
        // while (true)
            ;
    }

    // set output power to 22 dBm (accepted range is -17 - 22 dBm)
    if (radio.setOutputPower(22) == RADIOLIB_ERR_INVALID_OUTPUT_POWER)
    {
        Serial.println(F("Selected output power is invalid for this module!"));
        // while (true)
            ;
    }

    // set over current protection limit to 80 mA (accepted range is 45 - 240 mA)
    // NOTE: set value to 0 to disable overcurrent protection
    if (radio.setCurrentLimit(140) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT)
    {
        Serial.println(F("Selected current limit is invalid for this module!"));
        // while (true)
            ;
    }

    // set LoRa preamble length to 15 symbols (accepted range is 0 - 65535)
    if (radio.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH)
    {
        Serial.println(F("Selected preamble length is invalid for this module!"));
        // while (true)
            ;
    }

    // disable CRC
    if (radio.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION)
    {
        Serial.println(F("Selected CRC is invalid for this module!"));
        // while (true)
            ;
    }

    // Some SX126x modules have TCXO (temperature compensated crystal
    // oscillator). To configure TCXO reference voltage,
    // the following method can be used.
    if (radio.setTCXO(2.4) == RADIOLIB_ERR_INVALID_TCXO_VOLTAGE)
    {
        Serial.println(F("Selected TCXO voltage is invalid for this module!"));
        // while (true)
            ;
    }

    // Some SX126x modules use DIO2 as RF switch. To enable
    // this feature, the following method can be used.
    // NOTE: As long as DIO2 is configured to control RF switch,
    //       it can't be used as interrupt pin!
    if (radio.setDio2AsRfSwitch() != RADIOLIB_ERR_NONE)
    {
        Serial.println(F("Failed to set DIO2 as RF switch!"));
        // while (true)
            ;
    }

    Serial.println(F("All settings succesfully changed!"));

    // start transmitting the first packet
    Serial.print(F("[SX1262] Sending first packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    transmissionState = radio.startTransmit("Hello World!");

    return true;
}

static bool sd_card_init(void)
{
    if(!SD.begin(SD_CS)){
        Serial.println("Card Mount Failed");
        return false;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    return true;
}

static bool gps_init(void)
{
    int i = 10;
    bool reply = false;

    SerialGPS.begin(38400, SERIAL_8N1, BOARD_GPS_RXD, BOARD_GPS_TXD);

    Serial.println("\nTesting GPS Modem Response...\n");
    Serial.println("****");
    while (i)
    {
        if (SerialGPS.available())
        {
            String r = SerialGPS.readString();
            SerialMon.println(r);
            if (r.length() >= 0)
            {
                reply = true;
                break;
            }
        }
        delay(500);
        i--;
    }
    return false;
}

void idf_setup() 
{
    int backlight = 0;

    gpio_hold_dis((gpio_num_t)TOUCH_RST);
    gpio_hold_dis((gpio_num_t)LORA_RST);
    gpio_deep_sleep_hold_dis();

    // lora and sd use the same spi, in order to avoid mutual influence;
    // before powering on, all CS signals should be pulled high and in an unselected state;
    pinMode(LORA_CS, OUTPUT);
    digitalWrite(LORA_CS, HIGH);
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    // Set the interrupt input to input pull-up
    if (PCA9535_INT > 0) {
        pinMode(PCA9535_INT, INPUT_PULLUP);
    }

    Serial.begin(115200);
    // // while (!Serial);

    SPI.begin(BOARD_SPI_SCLK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
    Wire.begin(BOARD_SDA, BOARD_SCL);

    pinMode(BL_EN, OUTPUT);
    ui_setting_get_backlight(&backlight);
    ui_setting_set_backlight(backlight);

    peri_buf[E_PERI_INK_POWER]  = false; 
    peri_buf[E_PERI_BQ25896]    = bq25896_init();   // PMU --- 0x6B
    peri_buf[E_PERI_BQ27220]    = bq27220_init();   // PMU --- 0x55
    peri_buf[E_PERI_RTC]        = rtc_pcf8563_init(); // RTC --- 0x51
    peri_buf[E_PERI_TOUCH]      = touch_gt911_init();  // Touch --- 0x5D;
    peri_buf[E_PERI_LORA]       = lora_sx1262_init();
    peri_buf[E_PERI_SD_CARD]    = sd_card_init();
    peri_buf[E_PERI_GPS]        = gps_init();

    screen_init();

    io_extend_lora_gps_power_on(true);

    xTaskCreate(gps_task, "gps_task", 1024 * 3, NULL, NFC_PRIORITY, &gps_handle);
    // xTaskCreate(lora_task, "lora_task", 1024 * 3, NULL, LORA_PRIORITY, &lora_handle);

    printf("LVGL Init\n");
    lv_port_disp_init();

    printf("LVGL UI Entry\n");
    ui_entry();
}

void idf_loop() 
{
    lv_task_handler();
    delay(1);
}