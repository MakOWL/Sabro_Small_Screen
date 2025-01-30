#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "images.h"
#include "fonts.h"
#include "com_structs.h"

lv_obj_t *data_screen;
lv_obj_t *comp_data;
lv_obj_t *power_data;
lv_obj_t *temp_data;
lv_obj_t *outdoor_data;
lv_obj_t *defrost_data;
lv_obj_t *refrig_data;

lv_obj_t *comp_data_label;

lv_obj_t *left_btn;
lv_obj_t *right_btn;

int current_panel_index = 0;

void left_right_btn_even(lv_event_t *e){
   lv_obj_t *btn = lv_event_get_target(e);  // Get the button that was clicked
    static int current_panel = 0;  // Track the currently visible panel

    if (btn == left_btn) {
        // Move to the previous panel (decrease index)
        current_panel--;
        if (current_panel < 0) current_panel = 5;  // Wrap around if we go below 0
    }
    else if (btn == right_btn) {
        // Move to the next panel (increase index)
        current_panel++;
        if (current_panel > 5) current_panel = 0;  // Wrap around if we go above 5
    }

    // Hide all panels first
    lv_obj_add_flag(comp_data, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(power_data, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(temp_data, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(outdoor_data, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(defrost_data, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(refrig_data, LV_OBJ_FLAG_HIDDEN);

    // Show the selected panel based on current_panel
    switch (current_panel) {
        case 0:
            lv_obj_clear_flag(comp_data, LV_OBJ_FLAG_HIDDEN);
            break;
        case 1:
            lv_obj_clear_flag(power_data, LV_OBJ_FLAG_HIDDEN);
            break;
        case 2:
            lv_obj_clear_flag(temp_data, LV_OBJ_FLAG_HIDDEN);
            break;
        case 3:
            lv_obj_clear_flag(outdoor_data, LV_OBJ_FLAG_HIDDEN);
            break;
        case 4:
            lv_obj_clear_flag(defrost_data, LV_OBJ_FLAG_HIDDEN);
            break;
        case 5:
            lv_obj_clear_flag(refrig_data, LV_OBJ_FLAG_HIDDEN);
            break;
    }
  

}

void create_data_screen() {
    if (data_screen != NULL) {
        lv_scr_load(data_screen); // Load the existing screen
        return;
    }
    data_screen = lv_obj_create(NULL);
    lv_obj_set_pos(data_screen, 0, 0);
    lv_obj_set_size(data_screen, 240, 320);
        // Menu button
    lv_obj_t *menu_btn = lv_btn_create(data_screen);
    lv_obj_set_pos(menu_btn, 4, 6);
    lv_obj_set_size(menu_btn, 33, 24);
    //lv_obj_set_scroll_dir(data_screen, LV_DIR_VER);
    lv_obj_clear_flag(data_screen,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_scroll_to(data_screen, 0, 100, LV_ANIM_OFF);

    lv_obj_t *parent_obj = data_screen;

    lv_obj_t *menu_label = lv_label_create(menu_btn);
    lv_label_set_text(menu_label, "");
    lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);
    

    // Additional elements for the data screen can be added here.
          comp_data = lv_img_create(parent_obj);
            //objects.compressor_data = obj;
            lv_obj_set_pos(comp_data, -45, -5);
            lv_obj_set_size(comp_data, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(comp_data, &img_data_panal0);
            //lv_obj_add_flag(comp_data, LV_OBJ_FLAG_HIDDEN); 
            lv_img_set_zoom(comp_data, 180);
            {
                lv_obj_t *parent_obj = comp_data;
                {
                    // data_compressor_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_compressor_label = obj;
                    lv_obj_set_pos(obj, 1, -96);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Compressor");
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff252222), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_compressor_rpm_unit
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_compressor_rpm_unit = obj;
                    lv_obj_set_pos(obj, 0, 40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "RPM");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_compressor_rpm
                    comp_data_label = lv_label_create(parent_obj);
                    //objects.data_compressor_rpm = obj;
                    lv_obj_set_pos(comp_data_label, 0, 0);
                    lv_obj_set_size(comp_data_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(comp_data_label, "4500");
                    lv_obj_set_style_text_color(comp_data_label, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(comp_data_label, &ui_font_seven_segment_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(comp_data_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(comp_data_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_compressor_running_mode
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_compressor_running_mode = obj;
                    lv_obj_set_pos(obj, 0, -40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Normal");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        
        {
            // power_data
            power_data = lv_img_create(parent_obj);
            //objects.power_data = obj;
            lv_obj_set_pos(power_data, -45, -5);
            lv_obj_set_size(power_data, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(power_data, &img_data_panal1);
            lv_obj_add_flag(power_data, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_zoom(power_data, 180);
            {
                lv_obj_t *parent_obj = power_data;
                {
                    // data_power_direct_voltage
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.data_power_direct_voltage = obj;
                    lv_obj_set_pos(obj, 0, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "DCV: 380");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_power_alternating_current
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.data_power_alternating_current = obj;
                    lv_obj_set_pos(obj, 0, -15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Amp: 8.9");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_power_alternating_voltage
                    lv_obj_t *obj = lv_label_create(parent_obj);
                 //   objects.data_power_alternating_voltage = obj;
                    lv_obj_set_pos(obj, 0, -40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Volt: 230.6");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_power_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_power_label = obj;
                    lv_obj_set_pos(obj, 0, -90);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Power");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_power_direct_current
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.data_power_direct_current = obj;
                    lv_obj_set_pos(obj, 0, 35);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "DCA: 5.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // temp_data
            temp_data = lv_img_create(parent_obj);
            //objects.temp_data = obj;
            lv_obj_set_pos(temp_data, -45, -5);
            lv_obj_set_size(temp_data, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(temp_data, &img_data_panal0);
            lv_obj_add_flag(temp_data, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_zoom(temp_data, 180);
            {
                lv_obj_t *parent_obj = temp_data;
                {
                    // data_temperature_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_temperature_label_1 = obj;
                    lv_obj_set_pos(obj, 2, -95);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Temperature");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff161515), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_temperature_room_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_temperature_room_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -25);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Room: 28.6");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_temperature_set_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_temperature_set_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Set: 25");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_temperature_coil_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.data_temperature_coil_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Coil: 11.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_temperature_delta_temperature_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                  //  objects.data_temperature_delta_temperature_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 25);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Delta: 5.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_temperature_delta_achieved_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_temperature_delta_achieved_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "DA: Yes");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // outdoor_data
            outdoor_data = lv_img_create(parent_obj);
            //objects.outdoor_data = obj;
            lv_obj_set_pos(outdoor_data, -45, -5);
            lv_obj_set_size(outdoor_data, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(outdoor_data, &img_data_panal0);
            lv_obj_add_flag(outdoor_data, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_zoom(outdoor_data, 180);
            {
                lv_obj_t *parent_obj = outdoor_data;
                {
                    // data_outdoor_error_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_outdoor_error_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Error: No");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_outdoor_delay_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_outdoor_delay_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 25);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Delay: No");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_outdoor_motor_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_outdoor_motor_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "ODM: On");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_outdoor_motor_running_status_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_outdoor_motor_running_status_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -25);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Motor: Off");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_outdoor_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_outdoor_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -90);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Outdoor");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff252222), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_outdoor_inv_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_outdoor_inv_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Inv: Cool");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // defrost_data
            defrost_data = lv_img_create(parent_obj);
           // objects.defrost_data = obj;
            lv_obj_set_pos(defrost_data, -45, -4);
            lv_obj_set_size(defrost_data, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(defrost_data, &img_data_panal1);
            lv_obj_add_flag(defrost_data, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_zoom(defrost_data, 180);
            {
                lv_obj_t *parent_obj = defrost_data;
                {
                    // data_defrost_cycle_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_defrost_cycle_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Cycle: Off");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_defrost_mode_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_defrost_mode_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Mode: On");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_defrost_check_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_defrost_check_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Check: Off");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_defrost_rv_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_defrost_rv_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 35);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "RV: Off");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff151515), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_defrost_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_defrost_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -90);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Defrost");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // refrig_data
            refrig_data = lv_img_create(parent_obj);
            //objects.refrig_data = obj;
            lv_obj_set_pos(refrig_data, -45, -5);
            lv_obj_set_size(refrig_data, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(refrig_data, &img_data_panal0);
            lv_obj_add_flag(refrig_data, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_zoom(refrig_data, 180);
            {
                lv_obj_t *parent_obj = refrig_data;
                {
                    // data_refrigeration_suction_pressure_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_refrigeration_suction_pressure_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 25);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "SP: 39.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_refrigeration_discharge_pressure_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_refrigeration_discharge_pressure_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 45);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "DP: 76");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_refrigeration_suction_line_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_refrigeration_suction_line_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "S Line: 39.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_refrigeration_discharge_line_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_refrigeration_discharge_line_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, 5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "D Line: 76");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_refrigeration_ambient_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_refrigeration_ambient_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -55);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "AT: 45.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_refrigeration_liquid_line_temp_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    //objects.data_refrigeration_liquid_line_temp_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -35);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "L Line: 31.5");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // data_refrigeration_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                   // objects.data_refrigeration_label_1 = obj;
                    lv_obj_set_pos(obj, 0, -90);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Refrigeration");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff161515), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // left_btn
            left_btn = lv_btn_create(parent_obj);
            //objects.left_btn = obj;
            lv_obj_set_pos(left_btn, 30, 275);
            lv_obj_set_size(left_btn, 40, 30);
            lv_obj_add_event_cb(left_btn, left_right_btn_even, LV_EVENT_CLICKED, NULL);
            {
                lv_obj_t *parent_obj = left_btn;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // right_btn_1
            right_btn = lv_btn_create(parent_obj);
           // objects.right_btn_1 = obj;
            lv_obj_set_pos(right_btn, 160, 275);
            lv_obj_set_size(right_btn, 40, 30);
            lv_obj_add_event_cb(right_btn, left_right_btn_even, LV_EVENT_CLICKED, NULL);
            {
                lv_obj_t *parent_obj = right_btn;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    //update_data_screen(data);
}



        