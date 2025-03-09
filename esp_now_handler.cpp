#include "com_structs.h"
#include "widget_dec.h"
#include "image_colours.h"
#include "rtosTasks.h" 
#include "charts.h"


realTime_data data;
sending_data_t send_data;
uint8_t available_connections = 0;
uint8_t available_connections_macs[MAXIMUM_AVAILABLE_DEVICES_COUNT]
                                  [MAC_ADDRESS_ARRAY_SIZE] = {0};
uint8_t paired_mac[MAC_ADDRESS_ARRAY_SIZE] = {0};
uint8_t broadcast_mac[MAC_ADDRESS_ARRAY_SIZE] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char paired_device_name[PAIRED_DEVICE_NAME_CHARACTER_COUNT];
uint8_t incoming_mac[MAC_ADDRESS_ARRAY_SIZE];
ac_pairing_data_t incoming_pairing_data;
pairing_request_t incoming_pairing_request_data;
esp_now_peer_info_t peer_info;
uint8_t pairing_mac[MAC_ADDRESS_ARRAY_SIZE];
uint8_t pairing_stage;
bool is_paired = false;
bool is_pairing; 


void on_data_recv(const esp_now_recv_info_t *esp_now_info,const uint8_t *incoming_data, int len) {
  Serial.print("Data recieved\n");
  Serial.printf("Data length: %d\r\n", len);
  Serial.printf("Data Recieved from Mac Address %x:%x:%x:%x:%x:%x/n",esp_now_info->src_addr[0],esp_now_info->src_addr[1],esp_now_info->src_addr[2],esp_now_info->src_addr[3],esp_now_info->src_addr[4],esp_now_info->src_addr[5]);


  
  if (len == sizeof(ac_pairing_data_t) && pairing_stage == ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY) { 
    memcpy(&incoming_pairing_data, incoming_data,
           sizeof(incoming_pairing_data));
    memcpy(&incoming_mac, esp_now_info->src_addr,
           sizeof(uint8_t) * MAC_ADDRESS_ARRAY_SIZE);
  }
  if (len == sizeof(pairing_request_t)) {
    pairing_request_t *received_request = (pairing_request_t *)incoming_data;

    // Display the incoming message type
    Serial.print("Incoming message type: ");
    Serial.println(received_request->message_type);

    // Display the authenticator if needed
    Serial.print("Authenticator: ");
    Serial.println(received_request->authenticator);

    // Check specific message types
    if (received_request->authenticator == ESPNOW_MESSAGE_DATA_SABRO_AC_AUTHENTICATOR) {
      switch (received_request->message_type) {
        case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_SCREEN_STILL_IS_PAIRED_REQUEST:
          Serial.println("Message: AC Screen Still Paired Request");
          break;

        case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_PAIRED_WITH_SOME_OTHER_DEVICE_CONFIRMATION:
          Serial.println("Message: AC Paired with Some Other Device Confirmation");
          break;

        case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_BREAK_OLD_PAIR_REQUEST:
          Serial.println("Message: AC Break Old Pair Request");
          if (memcmp(esp_now_info->src_addr, pairing_mac, sizeof(pairing_mac)) == 0)
              pairing_stage = ESPNOW_PAIRING_STAGE_FORCE_PAIRING_REQUEST_RECIEVED_FROM_AC;
              Serial.println("Pairing Stage:");
              Serial.print((int)pairing_stage);
              // THIS IS WHERE FORCE PAIRING WILL OCCUR
          break;

        case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_PAIRING_SUCCESS:
          Serial.println("Message: Confirmation need message 5");
          pairing_stage = ESPNOW_PAIRING_STAGE_PAIRING_REQUEST_ACKNOWLEDGED;
          break;

        case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_UNPAIR_CONFIRMATION:
          Serial.println("Message: AC Unpair Confirmation");
          break;

        default:
          Serial.println("Message: Unknown Type");
          break;
      }
       if (len == sizeof(paired_device_name)) {
      strncpy(paired_device_name, (char *) incoming_data, sizeof(paired_device_name));
      }
    }
  // Display if the data matches specific simple message types
  if (len == sizeof(uint8_t)) {
    uint8_t message_type = *incoming_data;

    Serial.print("Simple message received: ");
    Serial.println(message_type);

    switch (message_type) {
      case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_UNPAIR_CONFIRMATION:
        Serial.println("Message: AC Unpair Confirmation");
        pairing_stage = ESPNOW_PAIRING_STAGE_AC_CONFIRMED_UNPAIRING;
        break;
      case ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_SCREEN_STILL_IS_PAIRED_REQUEST:
        Serial.println("Message: AC Screen Still Paired Request");
        pairing_stage = ESPNOW_PAIRING_STAGE_PAIR_ACKNOWLEDGEMENT_REQUEST_RECIEVED;
        break;

      default:
        Serial.println("Message: Unknown Simple Type");
        break;
    }
  }
  }
    if (is_paired && memcmp(esp_now_info->src_addr, paired_mac, sizeof(paired_mac)) == 0) {
    if ( len == sizeof(realTime_data)) {
      memcpy(&data, incoming_data, sizeof(realTime_data));
      Serial.println("Real-time data size matched");
      update_main_screen(data);
      update_data_screen(data);
      if(setting_screen != NULL){
        lv_obj_add_flag(pair_new_device_btn,LV_OBJ_FLAG_HIDDEN);// hide the pair button 
        lv_obj_clear_flag(unpair_btn,LV_OBJ_FLAG_HIDDEN);
      //  lv_label_set_text(paired_device_lbl, data.)
      }
      }
       if ( len == sizeof(paired_device_name)) {
      strncpy(paired_device_name, (char *)incoming_data, sizeof(paired_device_name));
      Serial.print("Paired Device Name Received: ");
      Serial.println(paired_device_name);
      lv_label_set_text(paired_device_lbl, paired_device_name);
    } 
    }
    if (len == sizeof(float) * (1 + DAILY_ARRAY_SIZE)) {
      if ((uint32_t) ((float *) incoming_data)[0] == ESPNOW_MESSAGE_TYPE_CHART_DAILY_OFF_PEAK_ARRAY) {
        // Directly copy data without using a semaphore
        memcpy(esp_now_daily_off_peak_units_array, incoming_data + sizeof(float),
              sizeof(float) * DAILY_ARRAY_SIZE);
      }
    }
     
}

void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Data sent to: ");
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac_addr[0], mac_addr[1], mac_addr[2],
             mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print(macStr);
  
    Serial.print(" | Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");

  Serial.print("Data Sent (Hex): ");
  for (int i = 0; i < last_sent_data_length; i++) {
      Serial.print(last_sent_data[i], HEX);
      Serial.print(" ");
  }
  Serial.print(" | Data Sent (Decimal): ");
  for (int i = 0; i < last_sent_data_length; i++) {
      Serial.print(last_sent_data[i]);
      Serial.print(" ");
  }
  Serial.print(" | Size: ");
  Serial.println(last_sent_data_length);
}



void esp_now_setup() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK){
      Serial.println("ESP NOW FAILED!!");
      return;
    }
    esp_now_register_recv_cb(esp_now_recv_cb_t(on_data_recv));
    initRTOS();
    esp_now_register_send_cb(on_data_sent);


  esp_now_peer_info_t broadcast_peer;
  memcpy(broadcast_peer.peer_addr, broadcast_mac, MAC_ADDRESS_ARRAY_SIZE);
  broadcast_peer.channel = 0;
  broadcast_peer.encrypt = false;
  broadcast_peer.ifidx = WIFI_IF_STA;
  if (esp_now_add_peer(&broadcast_peer) != ESP_OK) {
    Serial.print("Adding broadcast peer failed\r\n");
  }

  if (!is_paired)
    {
        // Register peer
        memcpy(peer_info.peer_addr, paired_mac, MAC_ADDRESS_ARRAY_SIZE);
        peer_info.channel = 0;
        peer_info.encrypt = false;
        peer_info.ifidx = WIFI_IF_STA;
        // Add peer
        if (esp_now_add_peer(&peer_info) != ESP_OK) {
          Serial.print("Failed to add peer\r\n");
          return;
        }
    }
 }

