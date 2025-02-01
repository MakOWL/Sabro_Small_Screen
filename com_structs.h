#ifndef COM_STRUCTS_H
#define COM_STRUCTS_H

#include <esp_now.h>
#include <WiFi.h>

enum {
  PAIRED_DEVICE_NAME_CHARACTER_COUNT = 20,
  PAIRED_DEVICE_SABRO_REMOTE_AUTHENTICATOR = 1184005015U,
  PAIRED_DEVICE_SABRO_AC_AUTHENTICATOR = 1184005016U,
  SMALL_SCREEN_AUTHENTICATOR = 1184005014U;
  MAXIMUM_AVAILABLE_DEVICES_COUNT = 10,
  MAC_ADDRESS_ARRAY_SIZE = 6
};


typedef struct realTime_data{
  uint8_t warning_code;

  uint8_t rtc_sec;
  uint8_t rtc_min;
  uint8_t rtc_hour;
  uint8_t rtc_day;
  uint8_t rtc_month;
  uint16_t rtc_year;

  uint8_t ble_byte_1;
  uint8_t ble_byte_2;
  uint8_t ble_byte_3;
  uint8_t ble_byte_4;
  uint8_t ble_byte_5;

  float temp;

  float coil_sensor;
  float room_sensor;
  float delta_temp;
  float suction_line_temp;
  float suction_line_pressure;
  float liquid_line_temp;
  float discharge_line_temp;
  float discharge_line_pressure;
  float outdoor_unit_ambient_temp;
  float alternating_current_volts;
  float dc_volts;
  float system_current;
  float fan_current;
  float compressor_current;
  uint16_t compressor_rpm;
  uint8_t inv_error;
  uint8_t inv_error_code;
  uint32_t ton_sec;
  uint32_t tof_sec;

  float hour_energy;
  float day_off_peak_energy;
  float day_peak_energy;
  float month_off_peak_energy;
  float month_peak_energy;
} realTime_data;

typedef struct pairing_data {
  char device_name[PAIRED_DEVICE_NAME_CHAR_COUNT];
  uint32_t authenticator;
} pairing_data_t;

enum { SEND_STRUCT_DATE_AND_TIME = 1, SEND_STRUCT_PAIRING_CONFIRMATION = 2 };

typedef struct pairing_confirmation_data {
  uint8_t target_structure;
  uint8_t is_paired;
  uint32_t authenticator;
} pairing_confirmation_data_t;

typedef struct requests_or_acknowledgement {
  uint8_t message_type;
  uint8_t message_data;
}requests_or_acknowledgement_t;

enum {
  ESPNOW_NO_REQUEST_OR_ACKNOWLEDGEMENT = 0,
  ESPNOW_MESSAGE_TYPE_REQUEST = 1,
  ESPNOW_MESSAGE_TYPE_ACKNOWLEDGEMENT = 2,
  ESPNOW_REQUEST_MASTER_DEVICE_NAME = 1,
};

extern uint8_t available_connections;
extern uint8_t available_connections_macs[MAXIMUM_AVAILABLE_DEVICES_COUNT]
                                         [MAC_ADDRESS_ARRAY_SIZE];
extern uint8_t paired_mac[MAC_ADDRESS_ARRAY_SIZE];
extern char paired_device_name[PAIRED_DEVICE_NAME_CHARACTER_COUNT];
extern uint8_t incoming_mac[MAC_ADDRESS_ARRAY_SIZE];
extern pairing_data_t incoming_pairing_data;
extern esp_now_peer_info_t peer_info;

extern bool is_paired;
extern bool is_pairing;

//extern bool data_received = false;
extern realTime_data data;
void esp_now_setup();
void update_main_screen(realTime_data data);
void update_data_screen(realTime_data data);

#endif