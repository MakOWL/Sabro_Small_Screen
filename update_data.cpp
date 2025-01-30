#include "com_structs.h"
#include "widget_dec.h"
#include "image_colours.h"


enum half_of_day {SECOND_HALF_OF_DAY = 1, HOURS_PER_HALF = 12};
static const char *month_names[12] = { "Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug",
                                     "Sep", "Oct", "Nov", "Dec" };

realTime_data data; // structure to recieve data 

void update_main_screen(realTime_data data){
  uint8_t R1 = bitRead(data.ble_byte_2, 7);
  uint8_t R2 = bitRead(data.ble_byte_2, 6);
  uint8_t R3 = bitRead(data.ble_byte_5, 7);
  lv_obj_set_style_img_recolor(img_cool_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_fan_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_dry_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_auto_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_heat_obj, default_color, LV_PART_MAIN);
    if (R3 == 1) {
      lv_obj_set_style_img_recolor(img_fan_obj, fan_color, LV_PART_MAIN);
      mode_index = 3;  
    }
    if (R1 == 0 && R2 == 0 && R3 == 0) {
      lv_obj_set_style_img_recolor(img_auto_obj, auto_color, LV_PART_MAIN);
      mode_index = 0;
    }
    if (R1 == 0 && R2 == 1 && R3 == 0) {
      lv_obj_set_style_img_recolor(img_heat_obj, heat_color, LV_PART_MAIN);
      mode_index = 2;
    }
    if (R1 == 1 && R2 == 0 && R3 == 0) {
      lv_obj_set_style_img_recolor(img_dry_obj, dry_color, LV_PART_MAIN);
      mode_index = 4;  
    }
    if (R1 == 1 && R2 == 1 && R3 == 0) {
      lv_obj_set_style_img_recolor(img_auto_obj, auto_color, LV_PART_MAIN);
      mode_index = 0;
    }
  
   lv_label_set_text(mode_label, modes[mode_index]); 
   lv_label_set_text_fmt(temp_label,"%.f",data.temp); 
   lv_label_set_text_fmt(date_time_label, "%.2d : %.2d %s - %.2d %s, %.4d",
                      data.rtc_hour % HOURS_PER_HALF, data.rtc_min,
                      data.rtc_hour / HOURS_PER_HALF == 1 ? "PM" : "AM",
                      data.rtc_day, 
                      (data.rtc_month != 0) ? month_names[data.rtc_month - 1] : month_names[data.rtc_month],
                      data.rtc_year);
   lv_arc_set_value(temp_dial, data.temp);
}

void update_data_screen(realTime_data data){


}