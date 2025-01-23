#ifndef SCREENS_H
#define SCREENS_H

#include <lvgl.h>

extern lv_obj_t *main_screen;
extern lv_obj_t *data_screen;
extern lv_obj_t *timer_screen;
extern lv_obj_t *temp_label;
extern lv_obj_t *temp_dial;
extern lv_obj_t *dropdown_list;

void create_main_screen();
void create_data_screen();
void create_timer_screen();

extern const char *modes[];

#endif

