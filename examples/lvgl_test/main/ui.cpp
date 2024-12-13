

#include "lvgl.h"

LV_IMG_DECLARE(img_battery)

lv_obj_t *img;

void img_switch_test_cb(lv_timer_t *t)
{
    static int cnt = 0;
    lv_coord_t offsx = lv_rand(0, LV_HOR_RES - 120);
    lv_coord_t offsy = lv_rand(0, LV_VER_RES - 120);

    lv_obj_align(img, LV_ALIGN_TOP_LEFT, offsx, offsy);
}

void ui_entry(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    disp->theme = lv_theme_mono_init(disp, false, LV_FONT_DEFAULT);

    lv_obj_t *obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    img = lv_img_create(obj);
    lv_img_set_src(img, &img_battery);
    lv_obj_center(img);

    lv_timer_create(img_switch_test_cb, 1500, NULL);
}


