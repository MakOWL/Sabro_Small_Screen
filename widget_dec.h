#ifndef SCREENS_H
#define SCREENS_H

#include <lvgl.h>
#include "com_structs.h"

//screens
extern lv_obj_t *main_screen;
extern lv_obj_t *data_screen;
extern lv_obj_t *timer_screen;
extern lv_obj_t *add_mod_screen;

//widgets main screen
extern lv_obj_t *temp_label;
extern lv_obj_t *temp_dial;
extern lv_obj_t *mode_label;
extern lv_obj_t *date_time_label;

//widgets data screen

//main screen mode images
extern lv_obj_t *img_auto_obj;
extern lv_obj_t *img_cool_obj;
extern lv_obj_t *img_heat_obj;
extern lv_obj_t *img_fan_obj;
extern lv_obj_t *img_dry_obj;

// creating screens
void create_main_screen();
void create_data_screen();
void create_timer_screen();
void create_additional_modes_screen();

void menu_button_event_handler(lv_event_t *e);// menu for all the screens


// main screen modes
extern const char *modes[];
extern int mode_index;

#endif

