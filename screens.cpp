#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "images.h"
#include "com_structs.h"
#include "image_colours.h"

#define DEMO_MODE 0
#define REAL_MODE 1
#define DEMO_OR_REAL_MODE REAL_MODE

// Global screen objects
lv_obj_t *main_screen;
lv_obj_t *temp_label;
lv_obj_t *temp_dial;
lv_obj_t *mode_label;

// images for modes
lv_obj_t *img_auto_obj;
lv_obj_t *img_cool_obj;
lv_obj_t *img_heat_obj;
lv_obj_t *img_fan_obj;
lv_obj_t *img_dry_obj;
lv_obj_t *power_img;
lv_obj_t *date_time_label;
lv_obj_t *room_temp_img;
lv_obj_t *amb_temp_img;
lv_obj_t *fan_speed_cont; // this is a container not an image 
lv_obj_t *eco_img;
lv_obj_t *swing_img;


// Mode options
const char *modes[] = {"Auto", "Cool", "Heat", "Fan", "Dry"};
int mode_index = 0;
static bool mode_label_clickable = true; // Flag to track if clickable
static lv_timer_t *reset_timer = NULL;   // Timer to reset the flag
uint16_t current_fan_speed = SEND_AC_FAN_SPEED_CLEAR;
//realTime_data data; 

lv_color_t auto_color = lv_color_hex(FIXED_COLOR(0xb1ff08));
lv_color_t cool_color = lv_color_hex(FIXED_COLOR(0x0000FF));  
lv_color_t heat_color = lv_color_hex(FIXED_COLOR(0xf83535));  
lv_color_t fan_color = lv_color_hex(FIXED_COLOR(0x40d7d7));
lv_color_t dry_color = lv_color_hex(FIXED_COLOR(0xfc9e2b));
lv_color_t default_color = lv_color_hex(FIXED_COLOR(0xFFFFFF));

  void action_image_pressed(lv_event_t *e)
  {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *calling_image = lv_event_get_target(e);
    //Serial.print("Pressed");

    if (event_code == LV_EVENT_PRESSED && calling_image != NULL)
      {
        lv_img_set_zoom(calling_image, 220);

        uint32_t i;
        for (i = 0; i < lv_obj_get_child_cnt(calling_image); i++) {
          lv_obj_t *child = lv_obj_get_child(calling_image, i);
          /*Do something with child*/
          if (child != NULL)
            {
              lv_obj_set_style_transform_zoom(child, 319, LV_PART_MAIN);
            }
        }
      }
  }
void action_image_released(lv_event_t *e){
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *calling_image = lv_event_get_target(e);
    //Serial.print("Released");
    if (event_code == LV_EVENT_RELEASED && calling_image != NULL)
      {
        lv_img_set_zoom(calling_image, 200);

        uint32_t i;
        for (i = 0; i < lv_obj_get_child_cnt(calling_image); i++) {
          lv_obj_t *child = lv_obj_get_child(calling_image, i);
          /*Do something with child*/
          if (child != NULL)
            {
              lv_obj_set_style_transform_zoom(child, 255, LV_PART_MAIN);
            }
        }
    }
}
#if DEMO_OR_REAL_MODE == DEMO_MODE

void action_send_data(lv_event_t *e) {
  lv_obj_t *img = lv_event_get_target(e);
  const void *src = lv_img_get_src(img);

  lv_obj_set_style_img_recolor(img, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_cool_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_fan_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_dry_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_auto_obj, default_color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor(img_heat_obj, default_color, LV_PART_MAIN);

  if (src == &img_cool) {
      lv_obj_set_style_img_recolor(img_cool_obj, cool_color, LV_PART_MAIN);
          mode_index = 1;
      } else if (src == &img_fan) {
          lv_obj_set_style_img_recolor(img_fan_obj, fan_color, LV_PART_MAIN);
          mode_index = 3;
      } else if (src == &img_dry) {
          lv_obj_set_style_img_recolor(img_dry_obj, dry_color, LV_PART_MAIN);
          mode_index = 4;
      } else if (src == &img_auto) {
          lv_obj_set_style_img_recolor(img_auto_obj, auto_color, LV_PART_MAIN);
          mode_index = 0;
      } else if (src == &img_heat) {
          lv_obj_set_style_img_recolor(img_heat_obj, heat_color, LV_PART_MAIN);
          mode_index = 2;
      }

      lv_label_set_text(mode_label, modes[mode_index]);
}

#elif DEMO_OR_REAL_MODE == REAL_MODE
void action_send_data(lv_event_t *e){
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *calling_widget = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED)
    {
      if(calling_widget == img_auto_obj){send_data.status_bits = SEND_AC_RUNNING_MODE_AUTO;}
      if(calling_widget == img_cool_obj){send_data.status_bits = SEND_AC_RUNNING_MODE_COOL;}
      if(calling_widget == img_heat_obj){send_data.status_bits = SEND_AC_RUNNING_MODE_HEAT;}
      if(calling_widget == img_fan_obj){send_data.status_bits = SEND_AC_RUNNING_MODE_FAN;}
      if(calling_widget == img_dry_obj){send_data.status_bits = SEND_AC_RUNNING_MODE_DRY;} 
      if (calling_widget == power_img){
      send_data.status_bits = 0;
      bitWrite(send_data.status_bits, SEND_AC_POWER_STATUS_BIT,
                      !bitRead(data.ble_byte_1, 7));
      } else{      
      bitWrite(send_data.status_bits, SEND_AC_POWER_STATUS_BIT,
                bitRead(data.ble_byte_1, 7));
      }
     send_data.set_temperature = data.temp;
     esp_now_send(paired_mac, (uint8_t *)&send_data, sizeof(send_data));

     if (calling_widget == fan_speed_cont)
          {
            switch (current_fan_speed)
              {
              case SEND_AC_FAN_SPEED_LOW:
                send_data.status_bits = SEND_AC_FAN_SPEED_MEDIUM;
                break;
              case SEND_AC_FAN_SPEED_MEDIUM:
                send_data.status_bits = SEND_AC_FAN_SPEED_HIGH;
                break;
              case SEND_AC_FAN_SPEED_HIGH:
                send_data.status_bits = SEND_AC_FAN_SPEED_AUTO;
                break;
              case SEND_AC_FAN_SPEED_AUTO:
                send_data.status_bits = SEND_AC_FAN_SPEED_LOW;
                break;
              }
          }
        if (calling_widget == swing_img )
          bitWrite(send_data.status_bits, SEND_AC_SWING_STATUS_BIT,
                   !bitRead(data.ble_byte_1, 6));
        else
          bitWrite(send_data.status_bits, SEND_AC_SWING_STATUS_BIT,
                   bitRead(data.ble_byte_1, 6));           
 }
}
#endif

void action_temperature_increament_button(lv_event_t *e){
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target_label = (lv_obj_t *)lv_event_get_user_data(e);
    if (event_code == LV_EVENT_CLICKED) {
        uint32_t temp = strtol(lv_label_get_text(target_label), NULL, 10);
        temp = temp < MAX_SET_TEMPERATURE ? temp + 1 : temp;

      send_data.status_bits = 0;

      bitWrite(send_data.status_bits, SEND_AC_POWER_STATUS_BIT,
               bitRead(data.ble_byte_1, 7));
      bitWrite(send_data.status_bits, SEND_AC_SWING_STATUS_BIT,
               bitRead(data.ble_byte_1, 6));

      send_data.set_temperature = temp;
      esp_now_send (paired_mac, (uint8_t *) &send_data, sizeof (send_data));

    }

}

void action_temperature_decrement_button(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target_label = (lv_obj_t *)lv_event_get_user_data(e);
    if (event_code == LV_EVENT_CLICKED) {
        uint32_t temp = strtol(lv_label_get_text(target_label), NULL, 10);
        temp = temp > MIN_SET_TEMPERATURE ? temp - 1 : temp;

      send_data.status_bits = 0;

      bitWrite(send_data.status_bits, SEND_AC_POWER_STATUS_BIT,
               bitRead(data.ble_byte_1, 7));
      bitWrite(send_data.status_bits, SEND_AC_SWING_STATUS_BIT,
               bitRead(data.ble_byte_1, 6));

      send_data.set_temperature = temp;
      esp_now_send (paired_mac, (uint8_t *) &send_data, sizeof (send_data));

    }
}

void action_temperature_increment_button(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target_label = (lv_obj_t *)lv_event_get_user_data(e);
    if (event_code == LV_EVENT_CLICKED) {
      uint16_t temp = strtol(lv_label_get_text(target_label), NULL, 10);
      temp = temp < MAX_SET_TEMPERATURE ? temp + 1 : temp;

      send_data.status_bits = 0;

      bitWrite(send_data.status_bits, SEND_AC_POWER_STATUS_BIT,
               bitRead(data.ble_byte_1, 7));
      bitWrite(send_data.status_bits, SEND_AC_SWING_STATUS_BIT,
               bitRead(data.ble_byte_1, 6));

      send_data.set_temperature = temp;

      esp_now_send (paired_mac, (uint8_t *) &send_data, sizeof (send_data));

    }
}



// Event Handlers
void menu_button_event_handler(lv_event_t *e) {
    side_menu(); // Handle menu visibility
}

void mode_label_event_handler(lv_event_t *e) {
     if (!mode_label_clickable) {
        return; // Ignore input if still in delay period
    }
    mode_label_clickable = false;

    // Start a timer for a delay (e.g., 1000 ms)
    //reset_timer = lv_timer_create(reset_clickable_timer, 1000, NULL);
    lv_obj_t *label = lv_event_get_target(e);
    mode_index = (mode_index + 1) % (sizeof(modes) / sizeof(modes[0]));
    lv_label_set_text(label, modes[mode_index]);
    Serial.print(modes[mode_index]);
}

void temp_handler(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *arc = lv_event_get_target(e);
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);
     if (event_code == LV_EVENT_VALUE_CHANGED) {
    send_data.status_bits = 0;
    bitWrite(send_data.status_bits, SEND_AC_POWER_STATUS_BIT,
              bitRead(data.ble_byte_1, 7));
    bitWrite(send_data.status_bits, SEND_AC_SWING_STATUS_BIT,
              bitRead(data.ble_byte_1, 6));
    send_data.set_temperature = lv_arc_get_value(arc);
    esp_now_send (paired_mac, (uint8_t *) &send_data, sizeof (send_data));

     if (label != NULL)
        {
          lv_label_set_text_fmt(label, "%d", lv_arc_get_value(arc));
        }
 }
}

void button_event_handler(lv_event_t *e) {
    lv_obj_t *button = lv_event_get_target(e);
    const char *button_text = lv_label_get_text(lv_obj_get_child(button, 0));
    int32_t value = lv_arc_get_value(temp_dial);

    if (strcmp(button_text, "+") == 0) value++;
    else if (strcmp(button_text, "-") == 0) value--;

    if (value < 0) value = 0;
    if (value > 30) value = 30;

    lv_arc_set_value(temp_dial, value);

    char value_str[8];
    sprintf(value_str, "%d", value);
    lv_label_set_text(temp_label, value_str);
}

// Main Screen
void create_main_screen() {
    if (main_screen != NULL) {
        lv_scr_load(main_screen); // Load the existing screen
        return;
    }
  
    main_screen = lv_obj_create(NULL);
    lv_obj_set_pos(main_screen, 0, 0);
    lv_obj_set_size(main_screen, 240, 320);
    lv_obj_set_scroll_dir(main_screen, LV_DIR_VER);
    lv_obj_t *container = lv_obj_create(main_screen);
    lv_obj_set_pos(container, 8, 30);
    lv_obj_set_size(container, 280, 70);
    lv_obj_set_style_pad_left(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_align(container, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_width(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *parent_obj = container;
      {
          // main_screen_ac_cool_mode_image_1
          img_cool_obj = lv_img_create(parent_obj);
          lv_obj_set_pos(img_cool_obj, -20, -6);
          lv_obj_set_size(img_cool_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_img_set_src(img_cool_obj, &img_cool);
          lv_img_set_pivot(img_cool_obj, 0, 0);
          lv_img_set_zoom(img_cool_obj, 200);
          lv_obj_add_event_cb(img_cool_obj, action_send_data,LV_EVENT_CLICKED,NULL);
          lv_obj_add_event_cb(img_cool_obj, action_image_pressed,LV_EVENT_PRESSED,NULL);
          lv_obj_add_event_cb(img_cool_obj, action_image_released,LV_EVENT_RELEASED,NULL);
          lv_obj_add_flag(img_cool_obj, LV_OBJ_FLAG_CLICKABLE);
          lv_obj_clear_flag(img_cool_obj, LV_OBJ_FLAG_SCROLLABLE);
          lv_obj_set_style_align(img_cool_obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_border_color(img_cool_obj, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_border_width(img_cool_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor(img_cool_obj, cool_color, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor_opa(img_cool_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
         // main_screen_ac_heat_mode_image_1
      img_heat_obj = lv_img_create(parent_obj);
      lv_obj_set_pos(img_heat_obj, 20, -6);
      lv_obj_set_size(img_heat_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
      lv_img_set_src(img_heat_obj, &img_heat);
      lv_img_set_pivot(img_heat_obj, 0, 0);
      lv_img_set_zoom(img_heat_obj, 200);
      lv_obj_add_flag(img_heat_obj, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(img_heat_obj, action_send_data,LV_EVENT_CLICKED,NULL);
      lv_obj_add_event_cb(img_heat_obj, action_image_pressed,LV_EVENT_PRESSED,NULL);
      lv_obj_add_event_cb(img_heat_obj, action_image_released,LV_EVENT_RELEASED,NULL);
      lv_obj_set_style_align(img_heat_obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor(img_heat_obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(img_heat_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      {
          // main_screen_ac_fan_mode_image_1
          img_fan_obj = lv_img_create(parent_obj);
          lv_obj_set_pos(img_fan_obj, 60, -7);
          lv_obj_set_size(img_fan_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_img_set_src(img_fan_obj, &img_fan);
          lv_img_set_pivot(img_fan_obj, 0, 0);
          lv_img_set_zoom(img_fan_obj, 200);
          lv_obj_add_flag(img_fan_obj, LV_OBJ_FLAG_CLICKABLE);
          lv_obj_add_event_cb(img_fan_obj, action_image_pressed,LV_EVENT_PRESSED,NULL);
          lv_obj_add_event_cb(img_fan_obj, action_image_released,LV_EVENT_RELEASED,NULL);
          lv_obj_add_event_cb(img_fan_obj, action_send_data, LV_EVENT_CLICKED, NULL);
          lv_obj_set_style_align(img_fan_obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor(img_fan_obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor_opa(img_fan_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
      {
          // main_screen_ac_dry_mode_image_1
          img_dry_obj = lv_img_create(parent_obj);
          lv_obj_set_pos(img_dry_obj, 100, -6);
          lv_obj_set_size(img_dry_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_img_set_src(img_dry_obj, &img_dry);
          lv_img_set_pivot(img_dry_obj, 0, 0);
          lv_img_set_zoom(img_dry_obj, 200);
          lv_obj_add_flag(img_dry_obj, LV_OBJ_FLAG_CLICKABLE);
          lv_obj_add_event_cb(img_dry_obj, action_image_pressed,LV_EVENT_PRESSED,NULL);
          lv_obj_add_event_cb(img_dry_obj, action_image_released,LV_EVENT_RELEASED,NULL);
          lv_obj_add_event_cb(img_dry_obj, action_send_data, LV_EVENT_CLICKED, NULL);
          lv_obj_set_style_align(img_dry_obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor(img_dry_obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor_opa(img_dry_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
      {
          // main_screen_ac_auto_mode_image_1
          img_auto_obj = lv_img_create(parent_obj);
          lv_obj_set_pos(img_auto_obj, -60, -6);
          lv_obj_set_size(img_auto_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_img_set_src(img_auto_obj, &img_auto);
          lv_img_set_pivot(img_auto_obj, 0, 0);
          lv_img_set_zoom(img_auto_obj, 200);
          lv_obj_add_event_cb(img_auto_obj, action_image_pressed,LV_EVENT_PRESSED,NULL);
          lv_obj_add_event_cb(img_auto_obj, action_image_released,LV_EVENT_RELEASED,NULL);
          lv_obj_add_flag(img_auto_obj, LV_OBJ_FLAG_CLICKABLE);
          lv_obj_add_event_cb(img_auto_obj, action_send_data, LV_EVENT_CLICKED, NULL);
          lv_obj_set_style_align(img_auto_obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor_opa(img_auto_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_img_recolor(img_auto_obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
      }


    date_time_label = lv_label_create(main_screen);
    lv_obj_set_pos(date_time_label, 65, 10);
    lv_obj_set_size(date_time_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(date_time_label, "10 Oct 2024, 6:30 pm");
    lv_obj_set_style_text_font(date_time_label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_align(date_time_label, LV_ALIGN_OUT_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Menu button
    lv_obj_t *menu_btn = lv_btn_create(main_screen);
    lv_obj_set_pos(menu_btn, 4, 6);
    lv_obj_set_size(menu_btn, 33, 24);
    lv_obj_t *menu_label = lv_label_create(menu_btn);
    lv_label_set_text(menu_label, "");
    lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);

    // Temperature dial
    temp_dial = lv_arc_create(main_screen);
    lv_obj_set_pos(temp_dial, 45, 100);
    lv_obj_set_size(temp_dial, 150, 170);
    lv_arc_set_range(temp_dial, 16, 30);
    lv_arc_set_value(temp_dial, 25);
    lv_arc_set_bg_end_angle(temp_dial, 45);
    lv_arc_set_mode(temp_dial, LV_ARC_MODE_NORMAL);

    temp_label = lv_label_create(main_screen);
    lv_obj_set_pos(temp_label, 90, 140);
    lv_label_set_text(temp_label, "25");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_46, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(temp_dial, temp_handler, LV_EVENT_VALUE_CHANGED, temp_label);
    

    // "+" Button
    lv_obj_t *inc_button = lv_btn_create(main_screen);
    lv_obj_set_pos(inc_button, 128, 236);
    lv_obj_set_size(inc_button, 40, 20);
    lv_obj_add_event_cb(inc_button, button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *inc_label = lv_label_create(inc_button);
    lv_label_set_text(inc_label, "+");
    lv_obj_add_event_cb(inc_button, action_temperature_increament_button,LV_EVENT_CLICKED, temp_label);
    lv_obj_set_style_align(inc_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(inc_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // "-" Button
    lv_obj_t *dec_button = lv_btn_create(main_screen);
    lv_obj_set_pos(dec_button, 62, 236);
    lv_obj_set_size(dec_button, 40, 20);
    lv_obj_add_event_cb(dec_button, button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *dec_label = lv_label_create(dec_button);
    lv_label_set_text(dec_label, "-");
    lv_obj_add_event_cb(dec_button, action_temperature_decrement_button, LV_EVENT_CLICKED, temp_label);
    lv_obj_set_style_align(dec_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dec_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Mode label
    mode_label = lv_label_create(main_screen);
    lv_obj_set_pos(mode_label, 90, 190);
    lv_label_set_text(mode_label, modes[1]);
    lv_obj_set_style_text_font(mode_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_add_flag(mode_label, LV_OBJ_FLAG_CLICKABLE);

    //Power button
    power_img = lv_img_create(main_screen);
    lv_obj_set_pos(power_img, 173, 251);
    lv_obj_set_size(power_img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_src(power_img, &img_power_button);
    lv_img_set_zoom(power_img, 200);
    lv_obj_add_flag(power_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(power_img, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(power_img, lv_color_hex(0xff3ce221), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(power_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(power_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(power_img, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(power_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(power_img, lv_color_hex(0xff167016), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(power_img, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(power_img, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(power_img, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(power_img, lv_color_hex(0xff167016), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(power_img, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(power_img, action_send_data, LV_EVENT_CLICKED, (void *) 0);
    lv_obj_add_event_cb(power_img, action_image_pressed,LV_EVENT_PRESSED,NULL);
    lv_obj_add_event_cb(power_img, action_image_released,LV_EVENT_RELEASED,NULL);
    lv_scr_load(main_screen);
}

