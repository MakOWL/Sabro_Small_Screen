#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "images.h"
#include "fonts.h"
#include "com_structs.h"
#include "eprom_utils.h"

lv_obj_t *setting_screen;
lv_obj_t *settings_screen_pairing_cont;
lv_obj_t *name_change_cont;
lv_obj_t *settings_screen_date_time_cont;

lv_obj_t *avail_devices_screen;
lv_obj_t *device_list;
lv_obj_t *device_list_buttons[MAXIMUM_AVAILABLE_DEVICES_COUNT];
lv_obj_t *paired_device_lbl;
lv_obj_t *pair_new_device_btn;
lv_obj_t *unpair_btn;
uint16_t unpairing_request_send_time;

lv_obj_t *left_btn_setting;
lv_obj_t *right_btn_setting;

int cont_count = 1;

void left_right_btn_event_settings_screen(lv_event_t *e){
   lv_obj_t *btn = lv_event_get_target(e);
    static int current_panel = 0;  

    if (btn == left_btn_setting) {
        current_panel--;
        if (current_panel < 0) current_panel = cont_count;
    }
    else if (btn == right_btn_setting) {
        current_panel++;
        if (current_panel > cont_count) current_panel = 0;
    }

    // Hide all panels first
    lv_obj_add_flag(settings_screen_pairing_cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(name_change_cont, LV_OBJ_FLAG_HIDDEN);
    // Show the selected panel based on current_panel
    switch (current_panel) {
        case 0:
            lv_obj_clear_flag(settings_screen_pairing_cont, LV_OBJ_FLAG_HIDDEN);
            break;
        case 1:
            lv_obj_clear_flag(name_change_cont, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}

void force_unpair_close(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
      pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
      lv_obj_del(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))));
    }
}

void force_unpair(lv_event_t *e){
  if(!is_paired) return;

    esp_now_peer_info_t peerInfo;
    if (esp_now_get_peer(paired_mac, &peerInfo) != ESP_OK) {
        Serial.println("Peer not found. Trying to add before deleting...");

        // Add the peer again before trying to delete it
        memset(&peerInfo, 0, sizeof(peerInfo));
        memcpy(peerInfo.peer_addr, paired_mac, sizeof(paired_mac));
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        
        if (esp_now_add_peer(&peerInfo) != ESP_OK) {
            Serial.println("Failed to add peer for deletion");
            return;
        }
    }
   
   if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    Serial.printf("Mac Address %x:%x:%x:%x:%x:%x/n",paired_mac[0],paired_mac[1],paired_mac[2],paired_mac[3],paired_mac[4],paired_mac[5]);
    if (esp_now_del_peer(paired_mac) != ESP_OK) {
       Serial.println("Failed to delete peer");
      return;
    }
    is_paired = false;
    pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
    eeprom_clear_mac_address();
    lv_obj_add_flag(unpair_btn,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(pair_new_device_btn,LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(paired_device_lbl,"Unpaired");
    lv_obj_del(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))));

}
}
void force_pair_close_button_action(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
    lv_obj_del(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))));
  }
}


void unpair_device_action(lv_event_t *e){
  // hadeling unpair 
  if(!is_paired){return;}
      uint8_t send_request = ESPNOW_MESSAGE_TYPE_SCREEN_UNPAIR_REQUEST;
      esp_now_send(paired_mac, (uint8_t *) &send_request, sizeof(send_request));
      Serial.print("Unpair Request sent");
      pairing_stage = ESPNOW_PAIRING_STAGE_SCREEN_REQUESTED_UNPAIRING;
      unpairing_request_send_time = millis();
}

void force_pair_action(lv_event_t *e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    pairing_request_t send_confirmation = {
        ESPNOW_MESSAGE_TYPE_SCREEN_BREAK_OLD_PAIR_CONFIRMATION,
        ESPNOW_MESSAGE_DATA_SABRO_SMALL_SCREEN_AUTHENTICATOR };
    esp_now_peer_info_t temp_peer;
    temp_peer.channel = 0;
    temp_peer.encrypt = false;
    temp_peer.ifidx = WIFI_IF_STA;
    memcpy(temp_peer.peer_addr, pairing_mac, sizeof(pairing_mac));
    if (esp_now_add_peer(&temp_peer) == ESP_OK) {
      esp_now_send(pairing_mac, (uint8_t *)&send_confirmation,
                   sizeof(send_confirmation));

      esp_now_del_peer(pairing_mac);
    }

    lv_obj_del(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))));
  }
}

void send_pair_request(lv_event_t *e){
    lv_obj_t *btn = lv_event_get_target(e);  
    uint32_t index = lv_obj_get_index(btn);  
    lv_obj_t *calling_button = lv_event_get_target(e);

    if (index < available_connections) {
        uint8_t *mac_address = available_connections_macs[index];  

        char mac_str[18];
        snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac_address[0], mac_address[1], mac_address[2],
                 mac_address[3], mac_address[4], mac_address[5]);
        printf("Paired Request to device with MAC address: %s\n", mac_str);  // Print MAC to serial
        memcpy(pairing_mac, mac_address, sizeof(pairing_mac));
        pairing_request_t send_request = {
          ESPNOW_MESSAGE_TYPE_SCREEN_PAIRING_REQUEST,
          ESPNOW_MESSAGE_DATA_SABRO_SMALL_SCREEN_AUTHENTICATOR
        };
         esp_now_peer_info_t temp_peer;
        memcpy(temp_peer.peer_addr, pairing_mac, sizeof(uint8_t) * MAC_ADDRESS_ARRAY_SIZE);
        temp_peer.channel = 0;
        temp_peer.encrypt = false;
        temp_peer.ifidx = WIFI_IF_STA;

        if (esp_now_add_peer(&temp_peer) != ESP_OK)
          {
            Serial.println("Failed to add temporary peer\r\n");
            return;
          }
        esp_now_send(pairing_mac, (uint8_t *)&send_request, sizeof(send_request));  // Send request to the device
        Serial.println("Message type 3 sent");
        esp_now_del_peer(pairing_mac);
        pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
    }
//lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))),LV_OBJ_FLAG_HIDDEN);
}

void action_pair_new_espnow_device_button(lv_event_t *e){
 lv_obj_clear_flag(avail_devices_screen, LV_OBJ_FLAG_HIDDEN);
 lv_obj_add_flag(left_btn_setting,LV_OBJ_FLAG_HIDDEN);
 lv_obj_add_flag(right_btn_setting,LV_OBJ_FLAG_HIDDEN);
 pairing_stage = ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY;
 Serial.println("Pairing state initiated");
 //update_setting_screen();
}

void action_pair_device_available_devices_close_button(lv_event_t *e){
  lv_obj_add_flag(avail_devices_screen, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(left_btn_setting,LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(right_btn_setting,LV_OBJ_FLAG_HIDDEN);
}

void create_setting_screen() {
     if (setting_screen != NULL) {
        lv_scr_load(setting_screen); // Load the existing screen
        return;
    }
    setting_screen = lv_obj_create(0);
    lv_obj_set_pos(setting_screen, 0, 0);
    lv_obj_t *menu_btn = lv_btn_create(setting_screen);
    lv_obj_set_pos(menu_btn, 4, 6);
    lv_obj_set_size(menu_btn, 33, 24);
    //lv_obj_set_scroll_dir(data_screen, LV_DIR_VER);
    lv_obj_clear_flag(setting_screen,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_scroll_to(setting_screen, 0, 100, LV_ANIM_OFF);
    lv_obj_t *menu_label = lv_label_create(menu_btn);
    lv_label_set_text(menu_label, "");
    lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(setting_screen, 240, 320);
    {
        lv_obj_t *parent_obj = setting_screen;
        {
            // settings_screen_pair_device_container
            settings_screen_pairing_cont = lv_obj_create(parent_obj);
            lv_obj_set_pos(settings_screen_pairing_cont, 40, 50);
            lv_obj_set_size(settings_screen_pairing_cont, LV_PCT(70), 220);
            lv_obj_set_style_pad_left(settings_screen_pairing_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(settings_screen_pairing_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(settings_screen_pairing_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(settings_screen_pairing_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(settings_screen_pairing_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(settings_screen_pairing_cont, lv_color_hex(0xff66708d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(settings_screen_pairing_cont, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(settings_screen_pairing_cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_obj_set_style_align(settings_screen_pairing_cont, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            //lv_obj_add_flag(settings_screen_pairing_cont,LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = settings_screen_pairing_cont;
                {
                    // settings_screen_pair_device_current_pair_container
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 32);
                    lv_obj_set_size(obj, LV_PCT(90), 107);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // settings_screen_pair_device_current_pair_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 24, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Current Pair:");
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // settings_screen_pair_device_current_pair_detail_label
                            lv_obj_t *paired_device_lbl = lv_label_create(parent_obj);
                            lv_obj_set_pos(paired_device_lbl, 0, 0);
                            lv_obj_set_size(paired_device_lbl, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(paired_device_lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
                            lv_label_set_text(paired_device_lbl, "No pair safhkjadsfhkjsfhkjsdhfkjsdhfkjsdhfshdfkshdfk");
                            lv_label_set_long_mode(paired_device_lbl,LV_LABEL_LONG_SCROLL);
                            lv_obj_set_style_text_font(paired_device_lbl, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(paired_device_lbl, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_left(paired_device_lbl, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(paired_device_lbl, 90, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // paired device 
                            paired_device_lbl = lv_label_create(parent_obj);
                            lv_obj_set_pos(paired_device_lbl, 0, 0);
                            lv_obj_set_size(paired_device_lbl, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(paired_device_lbl, "Unpaired");
                            lv_obj_set_style_text_font(paired_device_lbl, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(paired_device_lbl, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(paired_device_lbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // settings_screen_pair_device_warning_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, LV_PCT(10));
                    lv_obj_set_size(obj, LV_PCT(90), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Pairing a new device will unpair the current paired device");
                     lv_label_set_long_mode(obj,LV_LABEL_LONG_SCROLL);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffeb4030), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // settings_screen_pair_device_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 25, 15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Pair Devices");
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                    unpair_btn = lv_btn_create(parent_obj);
                    lv_obj_set_pos(unpair_btn, 0, LV_PCT(30));
                    lv_obj_set_size(unpair_btn, 130, 45);
                    lv_obj_add_event_cb(unpair_btn, unpair_device_action, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_color(unpair_btn, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(unpair_btn,LV_OBJ_FLAG_HIDDEN);
                    lv_obj_set_style_align(unpair_btn, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = unpair_btn;
                        {
                            // settings_screen_pair_device_unpair_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Unpair");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                {
                    // settings_screen_pair_device_new_pair_button
                    pair_new_device_btn = lv_btn_create(parent_obj);
                    lv_obj_set_pos(pair_new_device_btn, 0, LV_PCT(30));
                    lv_obj_set_size(pair_new_device_btn, 130, 45);
                    lv_obj_add_event_cb(pair_new_device_btn, action_pair_new_espnow_device_button, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_color(pair_new_device_btn, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                    //lv_obj_add_flag(pair_new_device_btn,LV_OBJ_FLAG_HIDDEN);
                    lv_obj_set_style_align(pair_new_device_btn, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = pair_new_device_btn;
                        {
                            // settings_screen_pair_device_new_pair_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                             lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Pair");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            // main_screen_pair_device_available_devices_container
            avail_devices_screen = lv_obj_create(setting_screen);
            lv_obj_set_pos(avail_devices_screen, 0, 0);
            lv_obj_set_size(avail_devices_screen, LV_PCT(90), LV_PCT(80));
            lv_obj_set_style_pad_left(avail_devices_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(avail_devices_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(avail_devices_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(avail_devices_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(avail_devices_screen, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(avail_devices_screen, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(avail_devices_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(avail_devices_screen, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(avail_devices_screen, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_border_color(avail_devices_screen, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(avail_devices_screen, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = avail_devices_screen;
                {
                    // main_screen_pair_device_available_devices_close_button
                    lv_obj_t *close_btn = lv_btn_create(parent_obj);
                    lv_obj_set_pos(close_btn, 0, -1);
                    lv_obj_set_size(close_btn, 55, 35);
                    lv_obj_add_event_cb(close_btn, action_pair_device_available_devices_close_button, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_align(close_btn, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = close_btn;
                        {
                            // main_screen_pair_device_available_devices_close_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Close");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // main_screen_pair_device_available_devices_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Available Devices");
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // settings_screen_pair_device_list
                    device_list = lv_list_create(parent_obj);
                    lv_obj_set_pos(device_list, 0, 0);
                    lv_obj_set_size(device_list, LV_PCT(90), LV_PCT(65));
                    lv_obj_set_style_align(device_list, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    for (int i = 0; i < MAXIMUM_AVAILABLE_DEVICES_COUNT; i++) {
                      
                      device_list_buttons[i] = lv_list_add_btn(device_list, NULL, "0");  // Add button to the list
                      lv_obj_add_flag(device_list_buttons[i], LV_OBJ_FLAG_HIDDEN);
                      lv_obj_add_event_cb(device_list_buttons[i], send_pair_request, LV_EVENT_CLICKED, NULL);  // Add event handler
                  }

    }
                }
            }
             //lv_obj_t *parent_obj = setting_screen;
        {
            // settings_screen_pair_device_container
            settings_screen_date_time_cont = lv_obj_create(parent_obj);
            lv_obj_set_pos(settings_screen_date_time_cont, 40, 50);
            lv_obj_set_size(settings_screen_date_time_cont, LV_PCT(70), 257);
            lv_obj_set_style_pad_left(settings_screen_date_time_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(settings_screen_date_time_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(settings_screen_date_time_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(settings_screen_date_time_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(settings_screen_date_time_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(settings_screen_date_time_cont, lv_color_hex(0xff66708d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(settings_screen_date_time_cont, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(settings_screen_date_time_cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(settings_screen_date_time_cont,LV_OBJ_FLAG_HIDDEN);
        } 
          
       }
      lv_obj_t *parent_obj = setting_screen;
       {
            // name_change_cont
            name_change_cont = lv_obj_create(parent_obj);
            lv_obj_set_pos(name_change_cont, 40, 50);
            lv_obj_set_size(name_change_cont, LV_PCT(70), 220);
            lv_obj_set_style_pad_left(name_change_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(name_change_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(name_change_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(name_change_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(name_change_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(name_change_cont, lv_color_hex(0xff66708d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(name_change_cont, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(name_change_cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(name_change_cont,LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = name_change_cont;
                {
                    // entered_name
                    lv_obj_t *entered_name = lv_textarea_create(parent_obj);
                    lv_obj_set_pos(entered_name, 10, 100);
                    lv_obj_set_size(entered_name, 150, 30);
                    lv_textarea_set_max_length(entered_name, 128);
                    lv_obj_clear_flag(entered_name,LV_OBJ_FLAG_SCROLLABLE);
                    lv_textarea_set_one_line(entered_name, false);
                    lv_textarea_set_password_mode(entered_name, false);
                }
                {
                    // settings_screen_pair_device_current_pair_container_1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 32);
                    lv_obj_set_size(obj, LV_PCT(90), 107);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                   /* {
                        lv_obj_t *parent_obj = obj;
                        {
                            // settings_screen_pair_device_current_pair_label_1
                            lv_obj_t *obj = lv_label_create(parent_obj);             
                            lv_obj_set_pos(obj, 24, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Current Pair");
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // paired device _1
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 24, -10);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "");
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }*/
                {
                    // settings_screen_pair_device_label_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 25, 15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Name change");
                    lv_obj_set_style_text_font(obj, &ui_font_hemi_head_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // name_change_btn
                    lv_obj_t *name_change_btn = lv_btn_create(parent_obj);
                    lv_obj_set_pos(name_change_btn, -19, LV_PCT(30));
                    lv_obj_set_size(name_change_btn, 130, 45);
                    //lv_obj_add_event_cb(name_change_btn, action_unpair_espnow_device_button_clicked, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_color(name_change_btn, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(name_change_btn, LV_ALIGN_RIGHT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = name_change_btn;
                        {
                            // settings_screen_pair_device_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Change Name");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
       left_btn_setting = lv_btn_create(setting_screen);
          //objects.left_btn = obj;
          lv_obj_set_pos(left_btn_setting, 40, 275);
          lv_obj_set_size(left_btn_setting, 40, 30);
          lv_obj_add_event_cb(left_btn_setting,left_right_btn_event_settings_screen, LV_EVENT_CLICKED, NULL);
          {
              lv_obj_t *parent_obj = left_btn_setting;
              {
                  lv_obj_t *obj = lv_label_create(parent_obj);
                  lv_obj_set_pos(obj, 0, 0);
                  lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                  lv_label_set_text(obj, "");
                  lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
              }
          }
            right_btn_setting = lv_btn_create(setting_screen);
            lv_obj_set_pos(right_btn_setting, 170, 275);
            lv_obj_set_size(right_btn_setting, 40, 30);
            lv_obj_add_event_cb(right_btn_setting,left_right_btn_event_settings_screen, LV_EVENT_CLICKED, NULL);
            {
                lv_obj_t *parent_obj = right_btn_setting;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
          
    }
} 

    
