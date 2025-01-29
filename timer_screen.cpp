#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "images.h"
#include "fonts.h"


lv_obj_t *timer_screen;

void create_timer_screen(){
    timer_screen = lv_obj_create(0);
    //objects.timer = obj;
    lv_obj_set_pos(timer_screen, 0, 0);
    lv_obj_set_size(timer_screen, 240, 320);
    lv_obj_clear_flag(timer_screen,LV_OBJ_FLAG_SCROLLABLE); 
    {
        lv_obj_t *parent_obj = timer_screen;
        {
            lv_obj_t *menu_btn = lv_btn_create(parent_obj);
            lv_obj_set_pos(menu_btn, 4, 6);
            lv_obj_set_size(menu_btn, 33, 24);
            lv_obj_set_style_align(menu_btn, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);
            {
                lv_obj_t *parent_obj = menu_btn;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 72, 424);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Timer Screen");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 72, 440);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Timer Screen");
                }
            }
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, -81, -39);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_timer_panal);
            lv_img_set_zoom(obj, 120);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, -81, 115);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_timer_panal);
            lv_img_set_zoom(obj, 120);
        }
        {
            // timer_screen_timer_on_checkbox
            lv_obj_t *obj = lv_checkbox_create(parent_obj);
          //  objects.timer_screen_timer_on_checkbox = obj;
            lv_obj_set_pos(obj, -62, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_checkbox_set_text(obj, "Timer On");
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // timer_screen_repeat_on_checkbox
            lv_obj_t *obj = lv_checkbox_create(parent_obj);
          //  objects.timer_screen_repeat_on_checkbox = obj;
            lv_obj_set_pos(obj, 54, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_checkbox_set_text(obj, "Repeat");
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // timers_screen_start_time_label
            lv_obj_t *obj = lv_label_create(parent_obj);
          //  objects.timers_screen_start_time_label = obj;
            lv_obj_set_pos(obj, -3, -97);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Start Time");
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // timers_screen_start_time_background_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
          //  objects.timers_screen_start_time_background_container = obj;
            lv_obj_set_pos(obj, -7, -58);
            lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // timers_screen_start_time_start_hour_text_area
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
             //       objects.timers_screen_start_time_start_hour_text_area = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_textarea_set_accepted_chars(obj, "1234567890");
                    lv_textarea_set_max_length(obj, 2);
                    lv_textarea_set_text(obj, "00");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                   // lv_obj_add_event_cb(obj, action_timer_screen_text_area_focused, LV_EVENT_FOCUSED, (void *)0);
                  //  lv_obj_add_event_cb(obj, action_timer_screen_text_area_defocused, LV_EVENT_DEFOCUSED, (void *)0);
                   // lv_obj_add_event_cb(obj, action_timer_screen_text_area_value_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
                {
                    // timers_screen_start_time_start_hour_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.timers_screen_start_time_start_hour_label = obj;
                    lv_obj_set_pos(obj, -35, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "00");
                   // lv_obj_add_event_cb(obj, action_timer_screen_time_labels_clicked, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // timers_screen_start_time_colon_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.timers_screen_start_time_colon_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, ":");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // timers_screen_start_time_start_minute_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.timers_screen_start_time_start_minute_label = obj;
                    lv_obj_set_pos(obj, 35, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "00");
                  //  lv_obj_add_event_cb(obj, action_timer_screen_time_labels_clicked, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // timers_screen_start_time_start_minute_text_area
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                  //  objects.timers_screen_start_time_start_minute_text_area = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_textarea_set_accepted_chars(obj, "1234567890");
                    lv_textarea_set_max_length(obj, 2);
                    lv_textarea_set_text(obj, "00");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    //lv_obj_add_event_cb(obj, action_timer_screen_text_area_focused, LV_EVENT_FOCUSED, (void *)0);
                    //lv_obj_add_event_cb(obj, action_timer_screen_text_area_defocused, LV_EVENT_DEFOCUSED, (void *)0);
                    //lv_obj_add_event_cb(obj, action_timer_screen_text_area_value_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }
        {
            // timers_screen_end_time_background_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
           // objects.timers_screen_end_time_background_container = obj;
            lv_obj_set_pos(obj, -2, 99);
            lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // timers_screen_end_time_end_hour_text_area
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                  //  objects.timers_screen_end_time_end_hour_text_area = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_textarea_set_accepted_chars(obj, "1234567890");
                    lv_textarea_set_max_length(obj, 2);
                    lv_textarea_set_text(obj, "00");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                   // lv_obj_add_event_cb(obj, action_timer_screen_text_area_focused, LV_EVENT_FOCUSED, (void *)0);
                    //lv_obj_add_event_cb(obj, action_timer_screen_text_area_defocused, LV_EVENT_DEFOCUSED, (void *)0);
                    //lv_obj_add_event_cb(obj, action_timer_screen_text_area_value_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
                {
                    // timers_screen_end_time_end_hour_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.timers_screen_end_time_end_hour_label = obj;
                    lv_obj_set_pos(obj, -35, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "00");
                   // lv_obj_add_event_cb(obj, action_timer_screen_time_labels_clicked, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // timers_screen_end_time_colon_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.timers_screen_end_time_colon_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, ":");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // timers_screen_end_time_end_minute_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.timers_screen_end_time_end_minute_label = obj;
                    lv_obj_set_pos(obj, 35, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "00");
                    //lv_obj_add_event_cb(obj, action_timer_screen_time_labels_clicked, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // timers_screen_end_time_end_minute_text_area
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                  //  objects.timers_screen_end_time_end_minute_text_area = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_textarea_set_accepted_chars(obj, "1234567890");
                    lv_textarea_set_max_length(obj, 2);
                    lv_textarea_set_text(obj, "00");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                   // lv_obj_add_event_cb(obj, action_timer_screen_text_area_focused, LV_EVENT_FOCUSED, (void *)0);
                   // lv_obj_add_event_cb(obj, action_timer_screen_text_area_defocused, LV_EVENT_DEFOCUSED, (void *)0);
                   // lv_obj_add_event_cb(obj, action_timer_screen_text_area_value_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }
        {
            // timers_screen_start_time_label_1
            lv_obj_t *obj = lv_label_create(parent_obj);
           // objects.timers_screen_start_time_label_1 = obj;
            lv_obj_set_pos(obj, 1, 56);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "End Time");
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}