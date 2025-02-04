#include "com_structs.h"
#include "widget_dec.h"
#include "image_colours.h"


enum half_of_day {SECOND_HALF_OF_DAY = 1, HOURS_PER_HALF = 12};
static const char *month_names[12] = { "Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug",
                                     "Sep", "Oct", "Nov", "Dec" };

 static uint32_t last_broadcast_time = 0;

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
      lv_obj_set_style_img_recolor(img_cool_obj, cool_color, LV_PART_MAIN);
      mode_index = 1;
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

void update_data_screen(realTime_data data) {
  // compressor data 
  lv_obj_t *rpm_lbl = lv_obj_get_child(comp_data, 2);
  lv_obj_t *comp_mode_lbl = lv_obj_get_child(comp_data, 3);
  if (rpm_lbl != NULL) {lv_label_set_text_fmt(rpm_lbl, "%u", data.compressor_rpm);}

   uint8_t R1 = bitRead(data.ble_byte_2, 7);
   uint8_t R2 = bitRead(data.ble_byte_2, 6);
   uint8_t R3 = bitRead(data.ble_byte_5, 7);

   R1 = bitRead(data.ble_byte_3, 7);
   R2 = bitRead(data.ble_byte_3, 6);
   R3 = bitRead(data.ble_byte_3, 5);
   bool saving_mode = bitRead(data.ble_byte_4, 3);

   if (R1 == 0 && R2 == 0 && R3 == 0)
        lv_label_set_text(comp_mode_lbl, "PStop");

    if (R1 == 0 && R2 == 0 && R3 == 1)
        lv_label_set_text(comp_mode_lbl, "DeFrosting");

    if (R1 == 0 && R2 == 1 && R3 == 0)
        lv_label_set_text(comp_mode_lbl, "Error");

    if (R1 == 0 && R2 == 1 && R3 == 1)
        lv_label_set_text(comp_mode_lbl, "Delta");

    if (R1 == 1 && R2 == 0 && R3 == 0)
        lv_label_set_text(comp_mode_lbl, "MChange");

    if (R1 == 1 && R2 == 0 && R3 == 1)
        lv_label_set_text(comp_mode_lbl, "Control");

    if (R1 == 1 && R2 == 1 && R3 == 0 && saving_mode == false)
        lv_label_set_text(comp_mode_lbl, "Normal");

    if (saving_mode == true)
        lv_label_set_text(comp_mode_lbl, "E.Saving");

    if (R1 == 1 && R2 == 1 && R3 == 1)
        lv_label_set_text(comp_mode_lbl, "F_Time");

    // outdoor 
    lv_obj_t *OBM_lbl = lv_obj_get_child(outdoor_data, 2);
    lv_obj_t *inv_lbl = lv_obj_get_child(outdoor_data, 5);
    lv_obj_t *motor_lbl = lv_obj_get_child(outdoor_data,3);
    lv_obj_t *delay_lbl = lv_obj_get_child(outdoor_data,1);
    
    R1 = bitRead(data.ble_byte_2, 3);
    R2 = bitRead(data.ble_byte_2, 2);
    R3 = bitRead(data.ble_byte_2, 1);

    if (R1 == 0 && R2 == 0 && R3 == 0)
        lv_label_set_text(OBM_lbl, "ODM: LCOF");

    if (R1 == 0 && R2 == 0 && R3 == 1)
        lv_label_set_text(OBM_lbl, "ODM: LCON");

    if (R1 == 0 && R2 == 1 && R3 == 0)
        lv_label_set_text(OBM_lbl, "ODM: HCON");

    if (R1 == 0 && R2 == 1 && R3 == 1)
        lv_label_set_text(OBM_lbl, "ODM: HOFF");

    if (R1 == 1 && R2 == 0 && R3 == 0)
        lv_label_set_text(OBM_lbl, "ODM: MCOF");

    if (R1 == 1 && R2 == 0 && R3 == 1)
        lv_label_set_text(OBM_lbl, "ODM: EOF");

    R1 = bitRead(data.ble_byte_5, 6);
    R2 = bitRead(data.ble_byte_5, 5);

    if (R1 == 0 && R2 == 0)
        lv_label_set_text(inv_lbl, "Inv: Off");

    if (R1 == 0 && R2 == 1)
        lv_label_set_text(inv_lbl, "Inv: Cool");

    if (R1 == 1 && R2 == 0)
        lv_label_set_text(inv_lbl, "Inv: Heat");

    lv_label_set_text(motor_lbl,bitRead(data.ble_byte_2, 0) ? "Motor: On" : "Motor: Off");
    lv_label_set_text(delay_lbl,bitRead(data.ble_byte_3, 0) ? "S Delay: Yes" : "S Delay: No");   
}


void update_setting_screen() {
  static uint32_t last_broadcast_time = 0;
  bool mac_already_exist = false;
  if (setting_screen == NULL)
    return;// if the screen is null then dont crash 
  
static const uint8_t empty_mac[MAC_ADDRESS_ARRAY_SIZE] = {0, 0, 0,
                                                                0, 0, 0};
      if (memcmp(incoming_mac, empty_mac,
                 sizeof(uint8_t) * MAC_ADDRESS_ARRAY_SIZE) != 0) {
        for (uint32_t i = 0; i < available_connections; i++) {
          if (memcmp(&available_connections_macs[i][0], incoming_mac,
                     sizeof(uint8_t) * MAC_ADDRESS_ARRAY_SIZE) == 0) {
            mac_already_exist = true;
            break;
          }
        }
                 }
  pairing_request_t send_request = {
          ESPNOW_MESSAGE_TYPE_PAIRING_AVAILABILITY_REQUEST,
          ESPNOW_MESSAGE_DATA_SABRO_SCREEN_AUTHENTICATOR};
      esp_now_send(broadcast_mac, (uint8_t *)&send_request,
                   sizeof(send_request));
      last_broadcast_time = millis();

    
 if (!mac_already_exist && strlen(incoming_pairing_data.device_name) > 0) {
    // Copy the incoming MAC address into the available connections array
    memcpy(&available_connections_macs[available_connections][0], incoming_mac, MAC_ADDRESS_ARRAY_SIZE * sizeof(uint8_t));

    // Update UI only if the device name is valid
    lv_label_set_text(lv_obj_get_child(device_list_buttons[available_connections], 0), incoming_pairing_data.device_name);
    lv_obj_clear_flag(device_list_buttons[available_connections], LV_OBJ_FLAG_HIDDEN);

    available_connections++;
}
}

