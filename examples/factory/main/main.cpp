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

// Arduino
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// Ink Screen
#define WAVEFORM EPD_BUILTIN_WAVEFORM
#define DEMO_BOARD epd_board_v7
EpdiyHighlevelState hl;
int temperature = 0;
uint8_t* fb = NULL;

static inline void checkError(enum EpdDrawError err);

TouchDrvGT911 touch;

#define DISP_BUF_SIZE (epd_rotated_display_width() * epd_rotated_display_height())
#define REFRESH_MODE_FAST   0
#define REFRESH_MODE_NORMAL 1
#define REFRESH_MODE_NEAT   2
int refresh_mode = REFRESH_MODE_FAST;

uint8_t *decodebuffer = NULL;
lv_timer_t *flush_timer = NULL;
volatile bool disp_flush_enabled = true;
bool disp_refr_is_busy = false;

static inline void checkError(enum EpdDrawError err) {
    if (err != EPD_DRAW_SUCCESS) {
        ESP_LOGE("demo", "draw error: %X", err);
    }
}

void ui_refresh_mode_chg(void)
{
    int mode = refresh_mode;

    mode++;
    if(mode > 2) {
        mode = 0;
    }
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

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
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

    if(refresh_mode == REFRESH_MODE_NORMAL) {
        disp_full_refresh();
    } else if(refresh_mode == REFRESH_MODE_NEAT){
        disp_full_clean();
    }
    
    epd_draw_rotated_image(rener_area, decodebuffer, epd_hl_get_framebuffer(&hl));
    epd_poweron();
    epd_hl_update_area(&hl, MODE_DU, epd_ambient_temperature(), rener_area);
    epd_poweroff();

    static int cnt = 0;
    // printf("[flush] %d\n", cnt++);

    lv_timer_pause(flush_timer);
}

static void dips_render_start_cb(struct _lv_disp_drv_t * disp_drv)
{
    if(flush_timer == NULL) {
        flush_timer = lv_timer_create(flush_timer_cb, 500, NULL);
        lv_timer_ready(flush_timer);
    } else {
        lv_timer_ready(flush_timer);
        lv_timer_resume(flush_timer);
    }
    printf("dips_render_start_cb\n");
}

void my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    // int16_t  x, y;

    int16_t x[5], y[5];
    if(touch.isPressed()) {
        if(touch.getPoint(x, y)){
            data->point.x = x[0];
            data->point.y = y[0];
            data->state = LV_INDEV_STATE_PRESSED;
            Serial.printf("input X:%d Y:%d\n", data->point.x, data->point.y);
        }else {
            data->state = LV_INDEV_STATE_RELEASED; 
        }
    }
}

void lv_port_disp_init(void)
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
    disp_drv.render_start_cb = dips_render_start_cb;
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

void touch_gt911_init(void)
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
        epd_hl_update_area(&hl, MODE_DU, epd_ambient_temperature(), rener_area);
        epd_poweroff();
    }, NULL);
}

void screen_init(void)
{
    epd_init(&DEMO_BOARD, &ED047TC1, EPD_LUT_64K);
    // Set VCOM for boards that allow to set this in software (in mV).
    // This will print an error if unsupported. In this case,
    // set VCOM using the hardware potentiometer and delete this line.
    epd_set_vcom(1560);

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
    epd_set_lcd_pixel_clock_MHz(12);

    heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
    heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);


    // epd_poweron();
    // epd_clear();
    // epd_poweroff();

    disp_full_clean();
    temperature = epd_ambient_temperature();

    printf("current temperature: %d\n", temperature);
}

void idf_setup() 
{
    Serial.begin(115200);
    // while (!Serial);

    Wire.begin(39, 40);

    touch_gt911_init();  // Touch --- 0x5D

    screen_init();

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