

#include "lvgl.h"
#include "scr_mrg.h"
#include "ui.h"
#include "ui_port.h"

//************************************[ Other fun ]******************************************
#if 1

void scr_back_btn_create(lv_obj_t *parent, const char *text, lv_event_cb_t cb)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_style_pad_all(btn, 0, 0);
    lv_obj_set_height(btn, 50);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 15, 15);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label2 = lv_label_create(btn);
    lv_obj_align(label2, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_color(label2, lv_color_hex(EPD_COLOR_TEXT), LV_PART_MAIN);
    lv_label_set_text(label2, " " LV_SYMBOL_LEFT);

    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align_to(label, btn, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_30, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(EPD_COLOR_TEXT), LV_PART_MAIN);
    lv_label_set_text(label, text);
    lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(label, cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_ext_click_area(label, 30);
}

// void scr_middle_line(lv_obj_t *parent)
// {
//     static lv_point_t line_points[] = { {LCD_HOR_SIZE/2, 0}, {LCD_HOR_SIZE/2, LCD_VER_SIZE-150}};
//     /*Create style*/
//     static lv_style_t style_line;
//     lv_style_init(&style_line);
//     lv_style_set_line_width(&style_line, 2);
//     lv_style_set_line_color(&style_line, lv_color_black());
//     lv_style_set_line_rounded(&style_line, true);
//     /*Create a line and apply the new style*/
//     lv_obj_t * line1;
//     line1 = lv_line_create(parent);
//     lv_line_set_points(line1, line_points, 2);     /*Set the points*/
//     lv_obj_add_style(line1, &style_line, 0);
//     lv_obj_set_align(line1, LV_ALIGN_LEFT_MID);
// }
#endif
//************************************[ screen 0 ]****************************************** menu
#if 1

const struct menu_icon icon_buf[] = {
    {&img_clock,    "clock"   , 45,   45  }, 
    {&img_lora,     "lora"    , 210,  45 },
    {&img_sd_card,  "sd card" , 375,  45 },
    {&img_setting,  "setting" , 45,   250  },
    {&img_test,     "test"    , 210,  250 },
    {&img_wifi,     "wifi"    , 375,  250 },
    {&img_battery,  "battery" , 45,   455  },
    {&img_shutdown, "shutdown", 210,  455 },
    {&img_refresh,  "refresh" , 375,  455 },
};

lv_obj_t *scr0_cont;

static void menu_btn_event(lv_event_t *e)
{
    int data = (int)e->user_data;
    if(e->code == LV_EVENT_CLICKED) {

        ui_full_refresh();
        // printf("%s is clicked.\n", icon_buf[data].icon_str);
        switch (data) {
            case 0: scr_mgr_push(SCREEN1_ID, false); break;
            case 1: scr_mgr_push(SCREEN2_ID, false); break;
            case 2: scr_mgr_push(SCREEN3_ID, false); break;
            case 3: scr_mgr_push(SCREEN4_ID, false); break;
            case 4: scr_mgr_push(SCREEN5_ID, false); break;
            case 5: scr_mgr_push(SCREEN6_ID, false); break;
            case 6: scr_mgr_push(SCREEN7_ID, false); break;
            case 7: scr_mgr_push(SCREEN8_ID, false); break;
            case 8: scr_mgr_push(SCREEN9_ID, false); break;
            default: break;
        }
    }
}

static void create0(lv_obj_t *parent) 
{
    for(int i = 0; i < sizeof(icon_buf)/sizeof(icon_buf[0]); i++) {
        lv_obj_t * btn = lv_btn_create(parent);
        lv_obj_remove_style_all(btn);
        lv_obj_set_width(btn, 120);
        lv_obj_set_height(btn, 160);
        lv_obj_set_x(btn, icon_buf[i].offs_x);
        lv_obj_set_y(btn, icon_buf[i].offs_y);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_outline_width(btn, 3, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_bg_img_src(btn, icon_buf[i].icon_src, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(btn, menu_btn_event, LV_EVENT_CLICKED, (void *)i);
    }
}
static void entry0(void) { }
static void exit0(void) { }
static void destroy0(void) { }

static scr_lifecycle_t screen0 = {
    .create = create0,
    .entry =   entry0,
    .exit  =   exit0,
    .destroy = destroy0,
};
#endif
//************************************[ screen 1 ]****************************************** clock
#if 1
static lv_obj_t  * calendar;
static lv_timer_t *get_timer = NULL;
static lv_meter_indicator_t * indic_min;
static lv_meter_indicator_t * indic_hour;
static lv_obj_t *clock_time;
static lv_obj_t *clock_data;
static lv_obj_t *clock_ap;
static lv_obj_t *clock_month;
static const char *week_list_en[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char * month_names_def[12] = LV_CALENDAR_DEFAULT_MONTH_NAMES;
static bool get_refresh_data(void)
{
    uint8_t h, m, s;
    uint8_t year, mont, day, week;

    ui_clock_get_time(&h, &m, &s);
    ui_clock_get_data(&year, &mont, &day, &week);

    if(h > 12) {
        lv_label_set_text_fmt(clock_ap, "%s", "P.M.");
    }  else {
        lv_label_set_text_fmt(clock_ap, "%s", "A.M.");
    }

    lv_calendar_set_today_date(calendar, 2000+year, mont, day);
    lv_calendar_set_showed_date(calendar, 2000+year, mont);
    lv_label_set_text_fmt(clock_month, "%s", month_names_def[mont-1]);

    lv_label_set_text_fmt(clock_time, "%02d:%02d", h%12, m);
    lv_label_set_text_fmt(clock_data, "20%02d-%02d-%02d  %s", year, mont, day, week_list_en[week]);

    printf("%2d:%2d:%02d-%d/%d/%d\n", h, m, s, year, mont, day);

    return year;
}

static void get_timer_event(lv_timer_t *t) 
{
    // refresh time per 65s
    bool is_ref = get_refresh_data();
    if(is_ref) {
        lv_timer_set_period(get_timer, 65*1000);
    }
}

static void scr1_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_full_refresh();
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create1(lv_obj_t *parent) {
    clock_time = lv_label_create(parent);
    clock_data = lv_label_create(parent);
    clock_ap = lv_label_create(parent);
    clock_month = lv_label_create(parent);

    lv_obj_set_style_border_width(clock_data, 2, 0);
    lv_obj_set_style_pad_top(clock_data, 30, 0);
    lv_obj_set_style_border_side(clock_data, LV_BORDER_SIDE_TOP, LV_PART_MAIN);

    lv_obj_set_style_text_font(clock_time, &Font_Mono_Bold_90, LV_PART_MAIN);
    lv_obj_set_style_text_font(clock_data, &Font_Mono_Bold_30, LV_PART_MAIN);
    lv_obj_set_style_text_font(clock_ap, &Font_Mono_Bold_30, LV_PART_MAIN);
    lv_obj_set_style_text_font(clock_month, &Font_Mono_Bold_30, LV_PART_MAIN);

    //---------------------
    calendar = lv_calendar_create(parent);
    lv_obj_set_size(calendar, 430, 380);
    lv_obj_set_style_text_font(calendar, &Font_Geist_Bold_20, LV_PART_MAIN);

    lv_obj_set_style_border_width(calendar, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(lv_calendar_get_btnmatrix(calendar), 0, LV_PART_ITEMS);
    lv_obj_set_style_border_side(lv_calendar_get_btnmatrix(calendar), LV_BORDER_SIDE_TOP, LV_PART_MAIN);

    //---------------------
    // scr_middle_line(parent);

    // back
    scr_back_btn_create(parent, "Clock", scr1_btn_event_cb); 
}
static void entry1(void) {
    // refresh time
    bool is_ref = get_refresh_data();
    if(is_ref) {
        get_timer = lv_timer_create(get_timer_event, 60*1000, NULL);
    } else {
        get_timer = lv_timer_create(get_timer_event, 6000, NULL);
    }
    
    // layout
    lv_obj_set_style_text_align(clock_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_align(clock_data, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_align(clock_ap, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_align(calendar, LV_ALIGN_TOP_MID, 0, 130);
    lv_obj_align_to(clock_month, calendar, LV_ALIGN_OUT_TOP_RIGHT, 0, -5);
    lv_obj_align_to(clock_time, calendar, LV_ALIGN_OUT_BOTTOM_MID, 0, 100);
    lv_obj_align_to(clock_data, clock_time, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    lv_obj_align_to(clock_ap, clock_time, LV_ALIGN_OUT_RIGHT_MID, 0, 20);
}
static void exit1(void) {
    if(get_timer) {
        lv_timer_del(get_timer);
        get_timer = NULL;
    }
}
static void destroy1(void) { }

static scr_lifecycle_t screen1 = {
    .create = create1,
    .entry = entry1,
    .exit  = exit1,
    .destroy = destroy1,
};
#endif
//************************************[ screen 2 ]****************************************** lora
#if 1
#define LORA_RECV_INFO_MAX_LINE 12

static int lora_mode_st = LORA_MODE_SEND;
static lv_obj_t *lora_mode_lab;
static lv_obj_t *lora_mode_sw;
static lv_obj_t *keyborad;
static lv_obj_t *textarea;
static lv_obj_t *cnt_label;
static lv_timer_t *lora_send_timer = NULL;

static int send_cnt = 0;
static int recv_cnt = 0;
static int lora_lab_cnt = 0;

static lv_obj_t *scr2_cont_info;
static lv_obj_t *lora_lab_buf[LORA_RECV_INFO_MAX_LINE] = {0};

static void scr2_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_full_refresh();
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static lv_obj_t * scr2_create_label(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, LCD_HOR_SIZE/2-50);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);   
    // lv_obj_set_style_border_width(label, 1, LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    return label;
}

static void lora_send_timer_event(lv_timer_t *t)
{
    if(lora_mode_st == LORA_MODE_SEND) return;
    
    String str = "";

    // if(lora_recv_success) {
    //     lora_recv_success = false;

    //     recv_cnt += strlen(lora_recv_data.c_str());

    //     lv_label_set_text_fmt(cnt_label, "R:%d", recv_cnt);

    //     str += lora_recv_data;

    //     lv_label_set_text_fmt(lora_lab_buf[0], "RECV: %ddBm", lora_recv_rssi);

    //     if(lora_lab_buf[lora_lab_cnt] == NULL) {
    //         lora_lab_buf[lora_lab_cnt] = scr2_create_label(scr2_cont_info);
    //         lv_label_set_text(lora_lab_buf[lora_lab_cnt], str.c_str());
    //     } else {
    //         lv_label_set_text(lora_lab_buf[lora_lab_cnt], str.c_str());
    //     }

    //     lora_lab_cnt++;
    //     if(lora_lab_cnt >= LORA_RECV_INFO_MAX_LINE) {
    //         lora_lab_cnt = 1;
    //     }
    // }
}

static void lora_mode_sw_event(lv_event_t * e)
{
    if(lora_mode_st == LORA_MODE_SEND)
    {
        lora_mode_st = LORA_MODE_RECV;
        lv_obj_add_flag(keyborad, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(textarea, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr2_cont_info, LV_OBJ_FLAG_HIDDEN);
        lv_timer_resume(lora_send_timer);
        lora_lab_cnt = 1;

        ui_lora_set_mode(LORA_MODE_RECV);
    } 
    else if(lora_mode_st == LORA_MODE_RECV) 
    {
        lora_mode_st = LORA_MODE_SEND;
        lv_obj_clear_flag(keyborad, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(textarea, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr2_cont_info, LV_OBJ_FLAG_HIDDEN);
        lv_timer_pause(lora_send_timer);

        ui_lora_set_mode(LORA_MODE_SEND);
    }

    lv_label_set_text_fmt(lora_mode_lab, "MODE : %s", (lora_mode_st == LORA_MODE_SEND)? "SEND" : "RECV");

}

static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        printf("LV_EVENT_VALUE_CHANGED\n");
    }

    if(code == LV_EVENT_READY)
    {
        printf("LV_EVENT_READY\n");
        if(ui_test_lora_init() == true) 
        {
            const char *str = lv_textarea_get_text(ta);
            int str_len = strlen(str);

            send_cnt += str_len;
            lv_label_set_text_fmt(cnt_label, "S:%d", send_cnt);
            ui_lora_transmit(str);
        }
        else 
        {
            printf("Not found LORA\n");
        }
        lv_textarea_set_text(ta,"");
    }
}

static void create2(lv_obj_t *parent) 
{
    /*Create a keyboard to use it with an of the text areas*/
    keyborad = lv_keyboard_create(parent);

    /*Create a text area. The keyboard will write here*/
    textarea = lv_textarea_create(parent);
    lv_obj_align(textarea, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_add_event_cb(textarea, ta_event_cb, LV_EVENT_VALUE_CHANGED, keyborad);
    lv_obj_add_event_cb(textarea, ta_event_cb, LV_EVENT_READY, keyborad);
    lv_obj_set_size(textarea, lv_pct(98), 150);
    lv_obj_clear_flag(textarea, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_letter_space(textarea, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(textarea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_keyboard_set_textarea(keyborad, textarea);

    scr2_cont_info = lv_obj_create(parent);
    lv_obj_set_size(scr2_cont_info, lv_pct(98), lv_pct(84));
    lv_obj_set_style_bg_color(scr2_cont_info, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr2_cont_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr2_cont_info, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(scr2_cont_info, 1, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr2_cont_info, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_left(scr2_cont_info, 50, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr2_cont_info, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(scr2_cont_info, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr2_cont_info, 5, LV_PART_MAIN);
    lv_obj_align(scr2_cont_info, LV_ALIGN_BOTTOM_MID, 0, -10);

    for(int i = 0; i < LORA_RECV_INFO_MAX_LINE; i++) {
        lora_lab_buf[i] = scr2_create_label(scr2_cont_info);
        lv_label_set_text(lora_lab_buf[i], " ");
    }
    lv_label_set_text(lora_lab_buf[0], "RECV : dBm");

    //
    lora_mode_sw = lv_btn_create(parent);
    lv_obj_set_style_radius(lora_mode_sw, 5, LV_PART_MAIN);
    lv_obj_set_style_border_width(lora_mode_sw, 2, LV_PART_MAIN);
    lora_mode_lab = lv_label_create(lora_mode_sw);
    lv_obj_set_style_text_font(lora_mode_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_obj_align(lora_mode_sw, LV_ALIGN_TOP_MID, 0, 22);
    lv_obj_add_event_cb(lora_mode_sw, lora_mode_sw_event, LV_EVENT_CLICKED, NULL);

    cnt_label = lv_label_create(parent);
    lv_obj_set_style_text_font(cnt_label, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_obj_align(cnt_label, LV_ALIGN_TOP_RIGHT, -30, 22);

    // back
    scr_back_btn_create(parent, "Lora", scr2_btn_event_cb);
}

static void entry2(void) 
{
    send_cnt = 0;
    recv_cnt = 0;

    lora_lab_cnt = 1;

    if(lora_mode_st == LORA_MODE_SEND)
    {
        lv_label_set_text(lora_mode_lab, "MODE : SEND");
        lv_label_set_text_fmt(cnt_label, "S:%d", send_cnt);
        lv_obj_clear_flag(keyborad, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(textarea, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr2_cont_info, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_label_set_text(lora_mode_lab, "MODE : RECV");
        lv_label_set_text_fmt(cnt_label, "R:%d", recv_cnt);
        lv_obj_add_flag(keyborad, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(textarea, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr2_cont_info, LV_OBJ_FLAG_HIDDEN);
    }

    lora_send_timer = lv_timer_create(lora_send_timer_event, 1000, NULL);
    if(lora_mode_st == LORA_MODE_SEND) 
    {
        lv_timer_pause(lora_send_timer);
    }
}

static void exit2(void) 
{

}
static void destroy2(void) 
{
    if(lora_send_timer) {
        lv_timer_del(lora_send_timer);
        lora_send_timer = NULL;
    }

    for(int i = 0; i < LORA_RECV_INFO_MAX_LINE; i++) {
        lora_lab_buf[i] = NULL;
    }
}

static scr_lifecycle_t screen2 = { 
    .create = create2,
    .entry = entry2,
    .exit  = exit2,
    .destroy = destroy2,
};
#endif
//************************************[ screen 3 ]****************************************** sd_card
#if 1

static lv_obj_t *scr3_cont_file;
static lv_obj_t *scr3_cont_img;
static lv_obj_t *sd_info;
static lv_obj_t *ui_photos_img;

static void read_img_btn_event(lv_event_t * e)
{
    char *file_name = lv_label_get_text((lv_obj_t *)e->user_data);

    if(e->code = LV_EVENT_CLICKED) {
        
        static char path[32];
        lv_snprintf(path, 32, "S:/%s", file_name);
        lv_img_set_src(ui_photos_img, path);
        printf("event [%s]\n", path);

        ui_full_refresh();
    }
}

static void scr3_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_full_refresh();
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void scr3_add_img_btn(const char *text, int text_len, int type)
{
    char buf[16] = {0};
    strncpy(buf, text, 16);
    char *suffix = (char *)text + text_len - 4;
    buf[text_len - 4] = '\0';

    printf("imgbtn [%s][%d][%s]\n", text, text_len, suffix);

    lv_obj_t *obj = lv_obj_create(scr3_cont_file);
    lv_obj_set_size(obj, LCD_HOR_SIZE/11, LCD_HOR_SIZE/11);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(obj);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
    lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);

    // switch (type) {
    //     case 1: lv_img_set_src(img, &img_JPG); break;
    //     case 2: lv_img_set_src(img, &img_PNG); break;
    //     case 3: lv_img_set_src(img, &img_BMP); break;
    //     default:
    //         break;
    // }

    lv_obj_t *lab = lv_label_create(obj);
    lv_obj_set_style_text_font(lab, &Font_Mono_Bold_20, LV_PART_MAIN);
    lv_label_set_text(lab, buf); // File suffixes are not displayed
    lv_obj_align_to(lab, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_t *lab1 = lv_label_create(obj);
    lv_label_set_text(lab1, text); 
    lv_obj_add_flag(lab1, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(img, read_img_btn_event, LV_EVENT_CLICKED, lab1);
}


static void create3(lv_obj_t *parent) {
    scr3_cont_file = lv_obj_create(parent);
    lv_obj_set_size(scr3_cont_file, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr3_cont_file, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr3_cont_file, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr3_cont_file, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr3_cont_file, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(scr3_cont_file, 5, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr3_cont_file, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(scr3_cont_file, 5, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr3_cont_file, 5, LV_PART_MAIN);
    lv_obj_set_align(scr3_cont_file, LV_ALIGN_BOTTOM_LEFT);

    scr3_cont_img = lv_obj_create(parent);
    lv_obj_set_size(scr3_cont_img, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr3_cont_img, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr3_cont_img, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr3_cont_img, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr3_cont_img, 0, LV_PART_MAIN);
    lv_obj_set_align(scr3_cont_img, LV_ALIGN_BOTTOM_RIGHT);

    //---------------------
    ui_photos_img = lv_img_create(scr3_cont_img);
    lv_obj_align(ui_photos_img, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *lab1;
    if(ui_test_sd_init()) {
        ui_sd_read();

        // //---------------------
        // scr_middle_line(parent);

        sd_info = lv_label_create(parent);
        lv_obj_set_style_text_font(sd_info, &Font_Mono_Bold_30, LV_PART_MAIN);
        lv_label_set_text(sd_info, "SD GALLERY"); 
    } else {
        sd_info = lv_label_create(parent);
        lv_obj_set_style_text_font(sd_info, &Font_Mono_Bold_30, LV_PART_MAIN);
        lv_label_set_text(sd_info, "NO FIND SD CARD!"); 
    }

    // back
    scr_back_btn_create(parent, "SD", scr3_btn_event_cb);
}
static void entry3(void) 
{
    // lv_obj_align(scr3_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    if(ui_test_sd_init()) {
        lv_obj_align(sd_info, LV_ALIGN_TOP_MID, 0, 22);
    } else {
        lv_obj_center(sd_info);
    }
}
static void exit3(void) { }
static void destroy3(void) {
}

static scr_lifecycle_t screen3 = {
    .create = create3,
    .entry = entry3,
    .exit  = exit3,
    .destroy = destroy3,
};
#endif
//************************************[ screen 4 ]****************************************** setting
#if 1
static void scr4_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED) {
        ui_full_refresh();
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create4(lv_obj_t *parent) 
{

    // back
    scr_back_btn_create(parent, "Setting", scr4_btn_event_cb);
}
static void entry4(void) { }
static void exit4(void) { }
static void destroy4(void) { }

static scr_lifecycle_t screen4 = {
    .create = create4,
    .entry = entry4,
    .exit  = exit4,
    .destroy = destroy4,
};
#endif
//************************************[ screen 5 ]****************************************** test
#if 1
static void scr5_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED) {
        ui_full_refresh();
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create5(lv_obj_t *parent) 
{

    // back
    scr_back_btn_create(parent, "Test", scr5_btn_event_cb);
}
static void entry5(void) { }
static void exit5(void) { }
static void destroy5(void) { }

static scr_lifecycle_t screen5 = {
    .create = create5,
    .entry = entry5,
    .exit  = exit5,
    .destroy = destroy5,
};
#endif
//************************************[ screen 6 ]****************************************** wifi
#if 1
lv_obj_t *scr6_root;
lv_obj_t *wifi_st_lab = NULL;
lv_obj_t *ip_lab;
lv_obj_t *ssid_lab;
lv_obj_t *pwd_lab;

static volatile bool smartConfigStart      = false;
static lv_timer_t   *wifi_timer            = NULL;
static uint32_t      wifi_timer_counter    = 0;
static uint32_t      wifi_connnect_timeout = 60;

static void wifi_info_label_create(lv_obj_t *parent)
{
    ip_lab = lv_label_create(parent);
    // lv_obj_set_style_text_color(ip_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(ip_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text_fmt(ip_lab, "ip: %s", ui_wifi_get_ip());
    lv_obj_align_to(ip_lab, wifi_st_lab, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    ssid_lab = lv_label_create(parent);
    // lv_obj_set_style_text_color(ssid_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(ssid_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text_fmt(ssid_lab, "ssid: %s", ui_wifi_get_ssid());
    lv_obj_align_to(ssid_lab, ip_lab, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    pwd_lab = lv_label_create(parent);
    // lv_obj_set_style_text_color(pwd_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(pwd_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text_fmt(pwd_lab, "pswd: %s", ui_wifi_get_pwd());
    lv_obj_align_to(pwd_lab, ssid_lab, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
}

static void wifi_config_event_handler(lv_event_t *e)
{
    static int step = 0;
    lv_event_code_t code  = lv_event_get_code(e);

    if(code != LV_EVENT_CLICKED) {
        return;
    }

    if(ui_wifi_get_status()){
        Serial.println(" WiFi is connected do not need to configure WiFi.");
        return;
    }

    if (smartConfigStart) {
        Serial.println("[wifi config] Config Stop");
        if (wifi_timer) {
            lv_timer_del(wifi_timer);
            wifi_timer = NULL;
        }
        WiFi.stopSmartConfig();
        Serial.println("return smart Config has Start;");
        smartConfigStart = false;
        return;
    }
    WiFi.disconnect();
    smartConfigStart = true;
    WiFi.beginSmartConfig();
    Serial.println("[wifi config] Config Start");
    lv_label_set_text(wifi_st_lab, "Wifi Config ...");
    
    wifi_timer = lv_timer_create([](lv_timer_t *t) {
        bool      destory = false;
        wifi_timer_counter++;
        if (wifi_timer_counter > wifi_connnect_timeout && !WiFi.isConnected()) {
            Serial.println("Connect timeout!");
            destory = true;
            Serial.println("[wifi config] Time Out");
        }
        if (WiFi.isConnected()) {
            Serial.println("WiFi has connected!");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());

            // if(strcmp(wifi_ssid, WiFi.SSID().c_str()) == 0) {
            //     Serial.printf("SSID == CURR SSID\r\n");
            // }
            // if(strcmp(wifi_password, WiFi.psk().c_str()) == 0) {
            //     Serial.printf("PSW == CURR PSW\r\n");
            // }
            
            // String ssid = WiFi.SSID();
            // String pwsd = WiFi.psk();
            // if(strcmp(wifi_ssid, ssid.c_str()) != 0 ||
            //    strcmp(wifi_password, pwsd.c_str()) != 0) {
            //     memcpy(wifi_ssid, ssid.c_str(), WIFI_SSID_MAX_LEN);
            //     memcpy(wifi_password, pwsd.c_str(), WIFI_PSWD_MAX_LEN);
            //     eeprom_wr_wifi(ssid.c_str(), ssid.length(), pwsd.c_str(), pwsd.length());
            // }

            destory   = true;
            String IP = WiFi.localIP().toString();
            ui_wifi_set_status(true);
            Serial.println("[wifi config] WiFi has connected!");

            lv_label_set_text(wifi_st_lab, (ui_wifi_get_status() == true ? "Wifi Connect" : "Wifi Disconnect"));
            wifi_info_label_create(scr6_root);
        }
        if (destory) {
            WiFi.stopSmartConfig();
            smartConfigStart = false;
            lv_timer_del(wifi_timer);
            wifi_timer         = NULL;
            wifi_timer_counter = 0;
        }
        // Every seconds check conected
    },
    1000, NULL);
}

static void scr6_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_full_refresh();
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create6(lv_obj_t *parent) {
    scr6_root = parent;
    wifi_st_lab = lv_label_create(parent);
    lv_obj_set_width(wifi_st_lab, 360);
    // lv_obj_set_style_text_color(wifi_st_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(wifi_st_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text(wifi_st_lab, (ui_wifi_get_status() ? "Wifi Connect" : "Wifi Disconnect"));
    lv_obj_set_style_text_align(wifi_st_lab, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(wifi_st_lab, LV_ALIGN_BOTTOM_MID, -0, -190);

    if(ui_wifi_get_status()) {
        wifi_info_label_create(parent);
    }

    lv_obj_t *label, *tips_label;
    tips_label = lv_label_create(parent);
    lv_obj_set_width(tips_label, LV_PCT(100));
    lv_label_set_long_mode(tips_label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_text_color(tips_label, lv_color_black(), LV_PART_MAIN);
    lv_label_set_text(tips_label,   " 1. Scan the QR code to download `EspTouch`\n\n"
                                    " 2. Install and launch `EspTouch` APP\n\n"
                                    " 3. Make sure your phone is \nconnected to WIFI\n\n"
                                    " 4. Tap the [EspTouch] option of the APP\n\n"
                                    " 5. Enter your WIFI password and click \n[confirm]\n\n"
                                    " 6. Finally, click [config wifi] on the\n ink screen\n\n"
                                    " After that, wait for the network \ndistribution to succeed!"
                                    );

    
    lv_obj_set_style_text_font(tips_label, &Font_Mono_Bold_20, LV_PART_MAIN);
    lv_obj_align(tips_label, LV_ALIGN_TOP_MID, 0, 100);

    const char *android_url = "https://github.com/EspressifApp/EsptouchForAndroid/releases/tag/v2.0.0/esptouch-v2.0.0.apk";
    const char *ios_url     = "https://apps.apple.com/cn/app/espressif-esptouch/id1071176700";

    lv_coord_t size            = 140;
    lv_obj_t  *android_rq_code = lv_qrcode_create(parent, size, lv_color_black(), lv_color_white());
    lv_qrcode_update(android_rq_code, android_url, strlen(android_url));
    lv_obj_set_pos(android_rq_code, 340, 10);
    lv_obj_align(android_rq_code, LV_ALIGN_LEFT_MID, 50, 100);

    lv_obj_set_style_border_color(android_rq_code, lv_color_white(), 0);
    lv_obj_set_style_border_width(android_rq_code, 5, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Android");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align_to(label, android_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t *ios_rq_code = lv_qrcode_create(parent, size, lv_color_black(), lv_color_white());
    lv_qrcode_update(ios_rq_code, ios_url, strlen(ios_url));
    lv_obj_align(ios_rq_code, LV_ALIGN_RIGHT_MID, -50, 100);

    lv_obj_set_style_border_color(ios_rq_code, lv_color_white(), 0);
    lv_obj_set_style_border_width(ios_rq_code, 5, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "IOS");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align_to(label, ios_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 200, 60);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -120);
    lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 10, LV_PART_MAIN);

    label = lv_label_create(btn);
    lv_label_set_text(label, "Config Wifi");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_obj_center(label);
    //---------------------
    // scr_middle_line(parent);
    // back
    scr_back_btn_create(parent, "Wifi", scr6_btn_event_cb);
}
static void entry6(void) { }
static void exit6(void) { }
static void destroy6(void) { }

static scr_lifecycle_t screen6 = {
    .create = create6,
    .entry = entry6,
    .exit  = exit6,
    .destroy = destroy6,
};

/*** UI interfavce ***/
String  __attribute__((weak)) ui_if_epd_get_WIFI_ip(void) {
    return "WIFI not connected";
}
const char * __attribute__((weak)) ui_if_epd_get_WIFI_ssid(void) {
    return "WIFI not connected";
}
const char * __attribute__((weak)) ui_if_epd_get_WIFI_pwd(void) {
    return "WIFI not connected";
}
// end
#endif
//************************************[ UI ENTRY ]******************************************
void ui_entry(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    disp->theme = lv_theme_mono_init(disp, false, LV_FONT_DEFAULT);

    scr_mgr_init();
    scr_mgr_set_bg_color(EPD_COLOR_BG);
    scr_mgr_register(SCREEN0_ID, &screen0); // menu
    scr_mgr_register(SCREEN1_ID, &screen1); // clock
    scr_mgr_register(SCREEN2_ID, &screen2); // lora
    scr_mgr_register(SCREEN3_ID, &screen3); // sd card
    scr_mgr_register(SCREEN4_ID, &screen4); // setting
    scr_mgr_register(SCREEN5_ID, &screen5); // test
    scr_mgr_register(SCREEN6_ID, &screen6); // wifi
    // scr_mgr_register(SCREEN7_ID, &screen7); // battery
    // scr_mgr_register(SCREEN8_ID, &screen8); // battery
    // scr_mgr_register(SCREEN9_ID, &screen9); // battery

    scr_mgr_switch(SCREEN0_ID, false); // set root screen
    scr_mgr_set_anim(LV_SCR_LOAD_ANIM_NONE, LV_SCR_LOAD_ANIM_NONE, LV_SCR_LOAD_ANIM_NONE);
}

