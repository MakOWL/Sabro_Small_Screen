#ifndef SCREENS_H
#define SCREENS_H

#include <lvgl.h>


extern lv_obj_t *main_screen;
extern lv_obj_t *data_screen;
extern lv_obj_t *timer_screen;
extern lv_obj_t *add_mod_screen;

//widgets
extern lv_obj_t *temp_label;
extern lv_obj_t *temp_dial;
extern lv_obj_t *dropdown_list;
extern lv_obj_t *mode_label;

void create_main_screen();
void create_data_screen();
void create_timer_screen();
void create_additional_modes_screen();
void menu_button_event_handler(lv_event_t *e);// menu for all the screens

extern const char *modes[];

#endif

