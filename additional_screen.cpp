#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "images.h"


lv_obj_t *add_mod_screen;

void create_additional_modes_screen(){
  if (add_mod_screen != NULL) {
        lv_scr_load(add_mod_screen); // Load the existing screen
        return;
    }
  
      add_mod_screen = lv_obj_create(NULL);
      lv_obj_t *mes_obj = lv_obj_create(add_mod_screen);
      lv_obj_set_pos(mes_obj, 10, 10);
      lv_obj_set_size(mes_obj, 240, 320);
      lv_obj_set_style_pad_left(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_top(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_right(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_bottom(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_border_width(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_radius(mes_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_t *menu_btn = lv_btn_create(add_mod_screen);
      lv_obj_set_pos(menu_btn, 4, 6);
      lv_obj_set_size(menu_btn, 33, 24);
      lv_obj_t *menu_label = lv_label_create(menu_btn);
      lv_label_set_text(menu_label, "");
      lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);

     // main_screen_room_temperature_image_1
      lv_obj_t *room_temp_img = lv_img_create(mes_obj);
      lv_obj_set_pos(room_temp_img, 0, 40);
      lv_obj_set_size(room_temp_img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
      lv_img_set_src(room_temp_img, &img_room_temperature);
      lv_img_set_zoom(room_temp_img, 160);
      lv_obj_set_style_align(room_temp_img, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
      {
          lv_obj_t *parent_obj = room_temp_img;
          {
              // main_screen_room_temperature_label_1
              lv_obj_t *obj = lv_label_create(parent_obj);
              lv_obj_set_pos(obj, 0, 14);
              lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
              lv_label_set_text(obj, "10");
              lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
              lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
          }
      }

      lv_obj_t *amb_temp_img = lv_img_create(mes_obj);
            lv_obj_set_pos(amb_temp_img, 160, 40);
            lv_obj_set_size(amb_temp_img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(amb_temp_img, &img_ambient_temperature);
            lv_img_set_zoom(amb_temp_img, 160);
            lv_obj_set_style_align(amb_temp_img, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = amb_temp_img;
                {
                    // main_screen_ambient_temperature_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 3, 13);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "10");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
    
    // main screen fan speed controller
     lv_obj_t *fan_speed_cont = lv_obj_create(mes_obj);
      //objects.main_screen_fan_speed_container_1 = obj;
      lv_obj_set_pos(fan_speed_cont, 0, 130);
      lv_obj_set_size(fan_speed_cont, LV_SIZE_CONTENT, 81);
      lv_obj_set_style_pad_left(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_top(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_right(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_bottom(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_border_width(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_radius(fan_speed_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      {
        lv_obj_t *parent_obj = fan_speed_cont;
        {
            // main_screen_fan_speed_image_1
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_fan_speed);
            lv_img_set_zoom(obj, 160);
            lv_img_set_angle(obj, 2);
            lv_obj_set_style_transform_angle(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // main_screen_fan_speed_label_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, -2);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "L");
            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 35, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_pos(obj, 1, -30);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    lv_obj_t *eco_img = lv_img_create(mes_obj);
    lv_obj_set_pos(eco_img, 0, 210);
    lv_obj_set_size(eco_img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_src(eco_img, &img_eco);
    lv_img_set_zoom(eco_img, 200);
    lv_obj_add_flag(eco_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_align(eco_img, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);

     // main_menu_swing_status_1
    lv_obj_t *swing_img = lv_img_create(mes_obj);
    lv_obj_set_pos(swing_img, 0, 260);
    lv_obj_set_size(swing_img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_src(swing_img, &img_swing);
    lv_img_set_zoom(swing_img, 200);
    lv_obj_set_style_img_recolor_opa(swing_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(swing_img, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    
}