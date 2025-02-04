#include "com_structs.h"
#include "widget_dec.h"
#include "image_colours.h"

realTime_data data; // structure to recieve data
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
  if (len == sizeof(ac_pairing_data_t) && pairing_stage == ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY) { 
    memcpy(&incoming_pairing_data, incoming_data,
           sizeof(incoming_pairing_data));
    memcpy(&incoming_mac, esp_now_info->src_addr,
           sizeof(uint8_t) * MAC_ADDRESS_ARRAY_SIZE);
  }
  Serial.printf("Pairing request lenght: %d\r\n",sizeof(pairing_request_t));
if (len == sizeof(pairing_request_t)) {
    Serial.print("Incoming message type: ");
    Serial.println(((pairing_request_t *) incoming_data)->message_type);
}

  if (len == sizeof(pairing_request_t) &&
    ((pairing_request_t *)incoming_data)->authenticator ==
        ESPNOW_MESSAGE_DATA_SABRO_AC_AUTHENTICATOR &&
    ((pairing_request_t *)incoming_data)->message_type ==
    ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_SCREEN_STILL_IS_PAIRED_REQUEST)
    
  if (is_paired && 
      memcmp(esp_now_info->src_addr, paired_mac, sizeof(paired_mac)) == 0)
    pairing_stage = ESPNOW_PAIRING_STAGE_PAIR_ACKNOWLEDGEMENT_REQUEST_RECIEVED;

  if (len == sizeof(pairing_request_t) &&
      ((pairing_request_t *)incoming_data)->authenticator ==
          ESPNOW_MESSAGE_DATA_SABRO_AC_AUTHENTICATOR &&
      ((pairing_request_t *)incoming_data)->message_type ==
      ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_PAIRED_WITH_SOME_OTHER_DEVICE_CONFIRMATION)
    if (memcmp(esp_now_info->src_addr, pairing_mac, sizeof(pairing_mac)) == 0)
      pairing_stage = ESPNOW_PAIRING_STAGE_AC_PAIRED_TO_OTHER_DEVICE_ACKNOWLEDGEMENT_RECIEVED;

  if (len == sizeof(pairing_request_t) &&
      ((pairing_request_t *)incoming_data)->authenticator ==
          ESPNOW_MESSAGE_DATA_SABRO_AC_AUTHENTICATOR &&
      ((pairing_request_t *)incoming_data)->message_type ==
      ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_BREAK_OLD_PAIR_REQUEST)
    if (memcmp(esp_now_info->src_addr, pairing_mac, sizeof(pairing_mac)) == 0)
      pairing_stage = ESPNOW_PAIRING_STAGE_FORCE_PAIRING_REQUEST_RECIEVED_FROM_AC;

  if (len == sizeof(pairing_request_t) &&
      pairing_stage == ESPNOW_PAIRING_STAGE_PAIRING_REQUEST_SENT &&
      memcmp(esp_now_info->src_addr, pairing_mac, sizeof(pairing_mac)) == 0)
    memcpy(&incoming_pairing_request_data, incoming_data,
           sizeof(incoming_pairing_request_data));

  if (len == sizeof(pairing_request_t) &&
      ((pairing_request_t *)incoming_data)->authenticator ==
          ESPNOW_MESSAGE_DATA_SABRO_AC_AUTHENTICATOR &&
      ((pairing_request_t *)incoming_data)->message_type ==
      ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_PAIRING_SUCCESS)
    if (memcmp(esp_now_info->src_addr, pairing_mac, sizeof(pairing_mac)) == 0)
      pairing_stage = ESPNOW_PAIRING_STAGE_PAIRING_REQUEST_ACKNOWLEDGED;

    if (is_paired &&memcmp(esp_now_info->src_addr, paired_mac, sizeof(paired_mac)) == 0) {
      if (len == sizeof(uint8_t))
        if (*incoming_data == ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_UNPAIR_CONFIRMATION)
          pairing_stage = ESPNOW_PAIRING_STAGE_AC_CONFIRMED_UNPAIRING;

        else if (*incoming_data == ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_SCREEN_STILL_IS_PAIRED_REQUEST)
          pairing_stage = ESPNOW_PAIRING_STAGE_PAIR_ACKNOWLEDGEMENT_REQUEST_RECIEVED;

    if (len == sizeof(paired_device_name)) {
      strncpy(paired_device_name, (char *) incoming_data, sizeof(paired_device_name));
      }

}
}

void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println("Data Sent");
}


void esp_now_setup() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK){
      Serial.println("ESP NOW FAILED!!");
      return;
    }
    esp_now_register_recv_cb(esp_now_recv_cb_t(on_data_recv));
    esp_now_register_send_cb(on_data_sent);

  esp_now_peer_info_t broadcast_peer;
  memcpy(broadcast_peer.peer_addr, broadcast_mac, MAC_ADDRESS_ARRAY_SIZE);
  broadcast_peer.channel = 0;
  broadcast_peer.encrypt = false;
  broadcast_peer.ifidx = WIFI_IF_STA;
  if (esp_now_add_peer(&broadcast_peer) != ESP_OK) {
    Serial.print("Adding broadcast peer failed\r\n");
  }

  if (is_paired)
    {
        // Register peer
        memcpy(peer_info.peer_addr, paired_mac, MAC_ADDRESS_ARRAY_SIZE);
        peer_info.channel = 0;
        peer_info.encrypt = false;

        // Add peer
        if (esp_now_add_peer(&peer_info) != ESP_OK) {
          Serial.print("Failed to add peer\r\n");
          return;
        }
    }
 }

