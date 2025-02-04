#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "images.h"
#include "fonts.h"
#include "com_structs.h"

lv_obj_t *setting_screen;
lv_obj_t *avail_devices_screen;
lv_obj_t *device_list;
lv_obj_t *device_list_buttons[MAXIMUM_AVAILABLE_DEVICES_COUNT];

void send_pair_request(lv_event_t *e){
    lv_obj_t *btn = lv_event_get_target(e);  // Get the button clicked
    uint32_t index = lv_obj_get_index(btn);  // Get the index of the button
    lv_obj_t *calling_button = lv_event_get_target(e);

    // Get the MAC address for the clicked device (from available_connections_macs)
    if (index < available_connections) {
        uint8_t *mac_address = available_connections_macs[index];  // Get the MAC address for this device

        // Convert MAC address to string
        char mac_str[18];
        snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac_address[0], mac_address[1], mac_address[2],
                 mac_address[3], mac_address[4], mac_address[5]);

        // Print the MAC address to Serial (or take any other action with it)
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
        esp_now_del_peer(pairing_mac);
        pairing_stage = ESPNOW_PAIRING_STAGE_NO_ACTIVITY;
    }
   /* for (uint32_t i = 0; i < MAXIMUM_AVAILABLE_DEVICES_COUNT; i++) {
          lv_obj_add_flag(device_list_buttons[i],LV_OBJ_FLAG_HIDDEN);
        }
        available_connections = 0;
        memset(available_connections_macs, 0, sizeof(available_connections_macs));
        memset(incoming_mac, 0, sizeof(incoming_mac));
        lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(calling_button)), LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lv_obj_get_screen(calling_button), LV_OBJ_FLAG_SCROLLABLE);*/

}

void action_pair_new_espnow_device_button(lv_event_t *e){
 lv_obj_clear_flag(avail_devices_screen, LV_OBJ_FLAG_HIDDEN);
 pairing_stage = ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY;
 update_setting_screen();
}

void action_pair_device_available_devices_close_button(lv_event_t *e){
  lv_obj_add_flag(avail_devices_screen, LV_OBJ_FLAG_HIDDEN);
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
    lv_obj_clear_flag(data_screen,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_scroll_to(data_screen, 0, 100, LV_ANIM_OFF);
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
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 36, 52);
            lv_obj_set_size(obj, LV_PCT(70), 257);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff66708d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
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
                            // settings_screen_pair_device_unpair_button
                            lv_obj_t *unpair_btn = lv_btn_create(parent_obj);
                            lv_obj_set_pos(unpair_btn, -43, 20);
                            lv_obj_set_size(unpair_btn, 65, 35);
                            lv_obj_set_style_bg_color(unpair_btn, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(unpair_btn, LV_ALIGN_RIGHT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // settings_screen_pair_device_unpair_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Unpair");
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
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
                            lv_obj_set_style_text_font(paired_device_lbl, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(paired_device_lbl, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_left(paired_device_lbl, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(paired_device_lbl, 90, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // paired device 
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 24, -10);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "");
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // settings_screen_pair_device_warning_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, LV_PCT(10));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Pairing a new device will unpair the current paired device");
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
                {
                    // settings_screen_pair_device_new_pair_button
                    lv_obj_t *pair_new_device_btn = lv_btn_create(parent_obj);
                    lv_obj_set_pos(pair_new_device_btn, 0, LV_PCT(30));
                    lv_obj_set_size(pair_new_device_btn, 130, 45);
                    lv_obj_add_event_cb(pair_new_device_btn, action_pair_new_espnow_device_button, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_color(pair_new_device_btn, lv_color_hex(0xff1960ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(pair_new_device_btn, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = pair_new_device_btn;
                        {
                            // settings_screen_pair_device_new_pair_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                             lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Pair new device");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            // main_screen_pair_device_available_devices_container
            avail_devices_screen = lv_obj_create(parent_obj);
            lv_obj_set_pos(avail_devices_screen, 0, 0);
            lv_obj_set_size(avail_devices_screen, LV_PCT(70), LV_PCT(70));
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
                    lv_obj_set_size(device_list, LV_PCT(70), LV_PCT(65));
                    lv_obj_set_style_align(device_list, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    for (int i = 0; i < MAXIMUM_AVAILABLE_DEVICES_COUNT; i++) {
                      
                      device_list_buttons[i] = lv_list_add_btn(device_list, NULL, "0");  // Add button to the list
                      lv_obj_add_flag(device_list_buttons[i], LV_OBJ_FLAG_HIDDEN);
                      lv_obj_add_event_cb(device_list_buttons[i], send_pair_request, LV_EVENT_CLICKED, NULL);  // Add event handler
                  }

    }
                }
            }
        }

//update_setting_screen();
    }
    
