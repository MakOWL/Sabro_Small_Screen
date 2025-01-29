#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_dial;
extern const lv_img_dsc_t img_ambient_temperature;
extern const lv_img_dsc_t img_auto;
extern const lv_img_dsc_t img_cool;
extern const lv_img_dsc_t img_dry;
extern const lv_img_dsc_t img_eco;
extern const lv_img_dsc_t img_fan;
extern const lv_img_dsc_t img_fan_speed;
extern const lv_img_dsc_t img_heat;
extern const lv_img_dsc_t img_menu;
extern const lv_img_dsc_t img_minus_button;
extern const lv_img_dsc_t img_plus_button;
extern const lv_img_dsc_t img_power_button;
extern const lv_img_dsc_t img_room_temperature;
extern const lv_img_dsc_t img_swing;
extern const lv_img_dsc_t img_data_panal0;
extern const lv_img_dsc_t img_data_panal1;
extern const lv_img_dsc_t img_timer_panal;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[18];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/