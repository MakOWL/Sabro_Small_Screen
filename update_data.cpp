#include "com_structs.h"
#include "widget_dec.h"
#include "image_colours.h"
#include "eprom_utils.h"


enum half_of_day {SECOND_HALF_OF_DAY = 1, HOURS_PER_HALF = 12};
static const char *month_names[12] = { "Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug",
                                     "Sep", "Oct", "Nov", "Dec" };

 static uint32_t last_broadcast_time = 0;

 #define MAX_DATA_SIZE 250  // Adjust based on the max size of your messages

uint8_t last_sent_data[MAX_DATA_SIZE];
size_t last_sent_data_length = 0;


void update_main_screen(realTime_data data){
  uint8_t R1 = bitRead(data.ble_byte_2, 7);
  uint8_t R2 = bitRead(data.ble_byte_2, 6);
  uint8_t R3 = bitRead(data.ble_byte_5, 7);
    if (bitRead(data.ble_byte_1, 7) == 1) {
    lv_obj_set_style_bg_color(power_img,
                              lv_color_hex(COLOR_LIMA),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(power_img,
                                   lv_color_hex(COLOR_BILBAO),
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(power_img,
                                  lv_color_hex(COLOR_BILBAO),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    lv_obj_set_style_bg_color(power_img,
                              lv_color_hex(COLOR_SILKEN_RUBY),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(power_img,
                                   lv_color_hex(COLOR_INCUBUS),
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(power_img,
                                  lv_color_hex(COLOR_INCUBUS),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
  }
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
   if(data.rtc_month > 12 || data.rtc_month < 0 ){
    data.rtc_month = 0;
   }
   lv_label_set_text(mode_label, modes[mode_index]); 
   lv_label_set_text_fmt(temp_label,"%.f",data.temp); 
   lv_label_set_text_fmt(date_time_label, "%.2d : %.2d %s - %.2d %s, %.4d",
                      data.rtc_hour % HOURS_PER_HALF, data.rtc_min,
                      data.rtc_hour / HOURS_PER_HALF == 1 ? "PM" : "AM",
                      data.rtc_day, 
                      (data.rtc_month != 0) ? month_names[data.rtc_month - 1] : month_names[data.rtc_month],
                      data.rtc_year);
  if(data.temp >= 16 && data.temp <= 30 )
   {
    lv_arc_set_value(temp_dial, data.temp);
    lv_label_set_text_fmt(temp_label,"%.f",data.temp);
    Serial.println("Temperature:");
    Serial.print(data.temp);  // Assuming data.temp is a float

   }
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
  
  // Defrost 
    lv_obj_t *cycle_lbl = lv_obj_get_child(defrost_data,0);
    lv_obj_t *mode_lbl = lv_obj_get_child(defrost_data,1);
    lv_obj_t *check_lbl = lv_obj_get_child(defrost_data,2);
    lv_obj_t *rv_lbl = lv_obj_get_child(defrost_data,3);
      
    if (bitRead(data.ble_byte_3, 3) == 1)
      lv_label_set_text(cycle_lbl, "Cycle: Yes");

    else
      lv_label_set_text(cycle_lbl, "Cycle: No");

    if (bitRead(data.ble_byte_3, 4) == 1)
      lv_label_set_text(mode_lbl, "Mode: Yes");
    else
      lv_label_set_text(mode_lbl, "Mode: No");

    if (bitRead(data.ble_byte_3, 2) == 1)
      lv_label_set_text(check_lbl,"Check:  Yes");
    else
      lv_label_set_text(check_lbl, "Check:  No");
    
    lv_label_set_text(rv_lbl,bitRead(data.ble_byte_4, 0) ? "RVS: On": "RVS: Off");

    // Power
    lv_obj_t *volt_lbl = lv_obj_get_child(power_data, 2);
    lv_label_set_text_fmt(volt_lbl,"Volt: %4.2f",data.alternating_current_volts);
    lv_obj_t *amp_lbl = lv_obj_get_child(power_data, 1);
    lv_label_set_text_fmt(amp_lbl,"Amp: %4.2f",data.system_current);
    lv_obj_t *dcv_lbl = lv_obj_get_child(power_data, 0);
    lv_label_set_text_fmt(dcv_lbl,"DCV: %4.2f",data.dc_volts);
    lv_obj_t *dca_lbl = lv_obj_get_child(power_data, 4);
    lv_label_set_text_fmt(dca_lbl,"DCA: %4.2f",data.compressor_current);

    // Temperature
    lv_obj_t *set_temp_lbl = lv_obj_get_child(temp_data, 2);
    lv_label_set_text_fmt(set_temp_lbl,"Set: %4.2f",data.temp);
    lv_obj_t *room_temp_lbl = lv_obj_get_child(temp_data, 1);
    lv_label_set_text_fmt(room_temp_lbl,"Room: %4.2f",data.room_sensor);
    lv_obj_t *coil_lbl = lv_obj_get_child(temp_data, 3);
    lv_label_set_text_fmt(coil_lbl,"Coil: %4.2f",data.coil_sensor);
    lv_obj_t *delta_lbl = lv_obj_get_child(temp_data, 4);
    lv_label_set_text_fmt(delta_lbl,"Delta: %4.2f",data.delta_temp);
    lv_obj_t *da_lbl = lv_obj_get_child(temp_data, 5);
    lv_label_set_text(da_lbl,bitRead(data.ble_byte_1, 1) ? "DA: Yes" : "DA: No");

    // Refrigeration
    lv_obj_t *at_lbl = lv_obj_get_child(refrig_data, 4);
    lv_obj_t *lline_lbl = lv_obj_get_child(refrig_data, 5);
    lv_obj_t *sline_lbl = lv_obj_get_child(refrig_data, 2);
    lv_obj_t *dline_lbl = lv_obj_get_child(refrig_data, 3);
    lv_obj_t *sp_lbl = lv_obj_get_child(refrig_data, 0);
    lv_obj_t *dp_lbl = lv_obj_get_child(refrig_data,1);

    lv_label_set_text_fmt(at_lbl,"AT: %4.2f",data.outdoor_unit_ambient_temp);
    lv_label_set_text_fmt(lline_lbl,"L Line: %4.2f", data.liquid_line_temp); 
    lv_label_set_text_fmt(sline_lbl,"S Line: %4.2f",data.suction_line_temp);
    lv_label_set_text_fmt(dline_lbl,"D Line: %4.2f",data.discharge_line_temp);
    if (data.suction_line_pressure != 0.00)
      lv_label_set_text_fmt(sp_lbl,"SP: %4.2f",data.suction_line_pressure);
    else
      lv_label_set_text(sp_lbl,"SP: Calibrating");

    if (data.discharge_line_pressure != 0)
      lv_label_set_text_fmt(dp_lbl,"DP: %4.2f",data.discharge_line_pressure);
    else
      lv_label_set_text(dp_lbl,"DP: Calibrating");

}


void update_setting_screen() {
  static uint32_t last_broadcast_time = 0;
  bool mac_already_exist = false;

  
  if (setting_screen == NULL)
    return;// if the screen is null then dont crash 

  if (pairing_stage == ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY &&
    ((millis() - last_broadcast_time) > ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY_BROADCAST_TIMEOUT)){

        memset(available_connections_macs, 0, sizeof(available_connections_macs));
        memset(incoming_mac, 0, sizeof(incoming_mac));

  for (uint8_t i = 0; i < available_connections; i++)
      lv_obj_add_flag(device_list_buttons[i], LV_OBJ_FLAG_HIDDEN);
      available_connections = 0;
      pairing_request_t send_request = {ESPNOW_MESSAGE_TYPE_PAIRING_AVAILABILITY_REQUEST,ESPNOW_MESSAGE_DATA_SABRO_SMALL_SCREEN_AUTHENTICATOR};
      esp_now_send(broadcast_mac, (uint8_t *)&send_request,sizeof(send_request));
      Serial.println("Message type 2 sent");
      last_broadcast_time = millis();
    }   

  for (uint8_t i = 0; i < available_connections; i++) {
    if (memcmp(available_connections_macs[i], incoming_mac, MAC_ADDRESS_ARRAY_SIZE) == 0) {
      mac_already_exist = true;
      break;
    }
  }

    
 if (!mac_already_exist && strlen(incoming_pairing_data.device_name) > 0) {
    // Copy the incoming MAC address into the available connections array
    memcpy(&available_connections_macs[available_connections][0], incoming_mac, MAC_ADDRESS_ARRAY_SIZE * sizeof(uint8_t));

    // Update UI only if the device name is valid
    lv_label_set_text(lv_obj_get_child(device_list_buttons[available_connections], 0), incoming_pairing_data.device_name);
    lv_obj_clear_flag(device_list_buttons[available_connections], LV_OBJ_FLAG_HIDDEN);

    available_connections++;
}
if (pairing_stage == ESPNOW_PAIRING_STAGE_SCREEN_REQUESTED_UNPAIRING &&
        (millis() - unpairing_request_send_time) >= ESPNOW_PAIRING_STAGE_UNPAIRING_REQUESTED_TIMEOUT)
      {
        lv_obj_t * unpairing_confirmation_message_box =
          lv_msgbox_create(NULL,  "Unpairing Confirmation","Are you sure", NULL, false);
        lv_obj_align(unpairing_confirmation_message_box, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_size(unpairing_confirmation_message_box, LV_PCT(60), LV_PCT(50));
        lv_obj_set_style_text_font(lv_msgbox_get_title(unpairing_confirmation_message_box), &lv_font_montserrat_18, LV_PART_MAIN);

        // force_unpair_confirm_button
        lv_obj_t *force_unpair_confirm_button =
            lv_btn_create(unpairing_confirmation_message_box);
        lv_obj_set_size(force_unpair_confirm_button, LV_SIZE_CONTENT, 35);
        lv_obj_set_style_bg_color(force_unpair_confirm_button,
                                  lv_color_hex(0xff1960ec),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(force_unpair_confirm_button, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_event_cb(force_unpair_confirm_button,force_unpair, LV_EVENT_CLICKED, (void *) 0);

        // force_unpair_confirm_label
        lv_obj_t *force_unpair_confirm_label =
            lv_label_create(force_unpair_confirm_button);
        lv_obj_set_size(force_unpair_confirm_label, LV_SIZE_CONTENT,
                        LV_SIZE_CONTENT);
        lv_label_set_text(force_unpair_confirm_label, "Force Unpair");

        // force_unpair_close_button
        lv_obj_t *force_unpair_close_button =
            lv_btn_create(unpairing_confirmation_message_box);
        lv_obj_set_size(force_unpair_close_button, LV_SIZE_CONTENT, 35);
        lv_obj_set_style_bg_color(force_unpair_close_button,
                                  lv_color_hex(0xff1960ec),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(force_unpair_close_button, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_event_cb(force_unpair_close_button, force_unpair_close, LV_EVENT_CLICKED, (void *) 0);

        // force_unpair_close_label
        lv_obj_t *force_unpair_close_label =
            lv_label_create(force_unpair_close_button);
        lv_obj_set_size(force_unpair_close_label, LV_SIZE_CONTENT,
                        LV_SIZE_CONTENT);
        lv_label_set_text(force_unpair_close_label, "Close");

        pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
    }
if (!is_paired &&
      pairing_stage == ESPNOW_PAIRING_STAGE_PAIRING_REQUEST_ACKNOWLEDGED) {
    if (is_paired)
      if (esp_now_del_peer(paired_mac) == ESP_OK)
        is_paired = false;

    if (!is_paired) {
      memcpy(paired_mac, pairing_mac, sizeof(paired_mac));
      // Register peer
      memcpy(peer_info.peer_addr, pairing_mac, MAC_ADDRESS_ARRAY_SIZE);
      peer_info.channel = 0;
      peer_info.encrypt = false;

      // Add peer
      if (esp_now_add_peer(&peer_info) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

      
      is_paired = true;
      eeprom_add_mac_address();

      pairing_stage = ESPNOW_PAIRING_STAGE_IS_PAIRED;
      pairing_request_t send_confirmation = {
          ESPNOW_MESSAGE_TYPE_SCREEN_PAIRING_SUCCESS,
          ESPNOW_MESSAGE_DATA_SABRO_SMALL_SCREEN_AUTHENTICATOR};

      memcpy(last_sent_data, (uint8_t *)&send_confirmation, sizeof(send_confirmation));
      last_sent_data_length = sizeof(send_confirmation);

      esp_now_send(paired_mac, (uint8_t *)&send_confirmation,
                   sizeof(send_confirmation));
      Serial.println("Message 5 has been sent");
      delay(200);
      lv_obj_add_flag(avail_devices_screen,LV_OBJ_FLAG_HIDDEN);
      //screen should hide here 
       
      uint8_t send_request = ESPNOW_MESSAGE_TYPE_REQUEST_MASTER_DEVICE_NAME;
      
      memcpy(last_sent_data, &send_request, sizeof(send_request));
      last_sent_data_length = sizeof(send_request);
      esp_now_send(paired_mac, (uint8_t *)&send_request, sizeof(send_request));

      pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
}
if (is_paired &&
      pairing_stage ==ESPNOW_PAIRING_STAGE_PAIR_ACKNOWLEDGEMENT_REQUEST_RECIEVED) {
    uint8_t send_confirmation = ESPNOW_MESSAGE_TYPE_SCREEN_STILL_IS_PAIRED_CONFIRMATION;
    esp_now_send(paired_mac, (uint8_t *)&send_confirmation,sizeof(send_confirmation));

    pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
  }

 }
  if (!is_paired &&
      pairing_stage == ESPNOW_PAIRING_STAGE_FORCE_PAIRING_REQUEST_RECIEVED_FROM_AC) {
        lv_obj_t * force_pairing_confirmation_message_box =
          lv_msgbox_create(NULL,  "Force Pairing Confirmation", "Are you sure", NULL, false);
        lv_obj_align(force_pairing_confirmation_message_box, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_size(force_pairing_confirmation_message_box, LV_PCT(60), LV_PCT(50));
        lv_obj_set_style_text_font(lv_msgbox_get_title(force_pairing_confirmation_message_box), &lv_font_montserrat_18, LV_PART_MAIN);

        // force_pair_confirm_button
        lv_obj_t *force_pair_confirm_button =
            lv_btn_create(force_pairing_confirmation_message_box);
        lv_obj_set_size(force_pair_confirm_button, LV_SIZE_CONTENT, 35);
        lv_obj_set_style_bg_color(force_pair_confirm_button,
                                  lv_color_hex(0xff1960ec),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(force_pair_confirm_button, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_event_cb(force_pair_confirm_button, force_pair_action, LV_EVENT_CLICKED, (void *) 0);

        // force_unpair_confirm_label
        lv_obj_t *force_pair_confirm_label =
            lv_label_create(force_pair_confirm_button);
        lv_obj_set_size(force_pair_confirm_label, LV_SIZE_CONTENT,
                        LV_SIZE_CONTENT);
        lv_label_set_text(force_pair_confirm_label, "Force pair");

        // force_unpair_close_button
        lv_obj_t *force_pair_close_button =
            lv_btn_create(force_pairing_confirmation_message_box);
        lv_obj_set_size(force_pair_close_button, LV_SIZE_CONTENT, 35);
        lv_obj_set_style_bg_color(force_pair_close_button,
                                  lv_color_hex(0xff1960ec),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(force_pair_close_button, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_event_cb(force_pair_close_button, force_pair_close_button_action, LV_EVENT_CLICKED, (void *) 0);

        // force_unpair_close_label
        lv_obj_t *force_pair_close_label =
            lv_label_create(force_pair_close_button);
        lv_obj_set_size(force_pair_close_label, LV_SIZE_CONTENT,
                        LV_SIZE_CONTENT);
        lv_label_set_text(force_pair_close_label, "Close");

        pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
  }
}

