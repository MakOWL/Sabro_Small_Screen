#ifndef WIDGET_DEC_H
#define WIDGET_DEC_H

#include <lvgl.h>
#include "com_structs.h"

//screens
extern lv_obj_t *main_screen;
extern lv_obj_t *data_screen;
extern lv_obj_t *timer_screen;
extern lv_obj_t *add_mod_screen;
extern lv_obj_t *setting_screen;

//widgets main screen
extern lv_obj_t *temp_label;
extern lv_obj_t *temp_dial;
extern lv_obj_t *mode_label;
extern lv_obj_t *date_time_label;


//widgets data screen
extern lv_obj_t *data_screen;
extern lv_obj_t *comp_data;
extern lv_obj_t *power_data;
extern lv_obj_t *temp_data;
extern lv_obj_t *outdoor_data;
extern lv_obj_t *defrost_data;
extern lv_obj_t *refrig_data;

extern lv_obj_t *left_btn;
extern lv_obj_t *right_btn;

//main screen mode images
extern lv_obj_t *img_auto_obj;
extern lv_obj_t *img_cool_obj;
extern lv_obj_t *img_heat_obj;
extern lv_obj_t *img_fan_obj;
extern lv_obj_t *img_dry_obj;
extern lv_obj_t *img_fan_speed_obj;
extern lv_obj_t *power_img;

// data screen stuff
extern lv_obj_t *comp_data_label;

// additional modes
extern lv_obj_t *room_temp_img;
extern lv_obj_t *amb_temp_img;
extern lv_obj_t *fan_speed_cont; // this is a container not an image 
extern lv_obj_t *eco_img;
extern lv_obj_t *swing_img;

// power monitor 
extern lv_obj_t *power_monitor_hourly_screen;
extern lv_obj_t *power_monitor_daily_screen;
extern lv_obj_t *power_monitor_monthly_screen;


// settings screen
extern lv_obj_t *avail_devices_screen;
extern lv_obj_t *device_list;
extern lv_obj_t *device_list_buttons[MAXIMUM_AVAILABLE_DEVICES_COUNT];
extern lv_obj_t *paired_device_lbl;
extern lv_obj_t *unpair_btn;
extern lv_obj_t *pair_new_device_btn;


// creating screens
void create_main_screen();
void create_data_screen();
void create_timer_screen();
void create_additional_modes_screen();
void create_screen_power_monitor_hourly_screen();
void create_screen_power_monitor_daily_screen(); 
void create_setting_screen();


void menu_button_event_handler(lv_event_t *e);// menu for all the screens
void action_image_pressed(lv_event_t *e);
void action_image_released(lv_event_t *e);



// main screen modes
extern const char *modes[];
extern int mode_index;

#endif

