#ifndef COM_STRUCTS_H
#define COM_STRUCTS_H

#include <esp_now.h>
#include <WiFi.h>
#include <lvgl.h>

 #define MAX_DATA_SIZE 250

enum {
  PAIRED_DEVICE_NAME_CHARACTER_COUNT = 20,
  PAIRED_DEVICE_SABRO_REMOTE_AUTHENTICATOR = 1184005015U,
  PAIRED_DEVICE_SABRO_AC_AUTHENTICATOR = 1184005016U,
  SMALL_SCREEN_AUTHENTICATOR = 1184005014U,
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

/*typedef struct pairing_data {
  char device_name[ PAIRED_DEVICE_NAME_CHARACTER_COUNT];
  uint32_t authenticator;
} pairing_data_t;*/

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
  ESPNOW_MESSAGE_TYPE_DATE_AND_TIME,
  ESPNOW_MESSAGE_TYPE_PAIRING_AVAILABILITY_REQUEST,
  ESPNOW_MESSAGE_TYPE_SCREEN_PAIRING_REQUEST,
  ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_PAIRING_SUCCESS,
  ESPNOW_MESSAGE_TYPE_SCREEN_PAIRING_SUCCESS,
  ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_SCREEN_STILL_IS_PAIRED_REQUEST,
  ESPNOW_MESSAGE_TYPE_SCREEN_STILL_IS_PAIRED_CONFIRMATION,
  ESPNOW_MESSAGE_TYPE_SCREEN_IS_NOT_PAIRED_CONFIRMATION, // Sent to an AC to tell it that its not paired with it anymore
  ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_PAIRED_WITH_SOME_OTHER_DEVICE_CONFIRMATION,
  ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_BREAK_OLD_PAIR_REQUEST,
  ESPNOW_MESSAGE_TYPE_SCREEN_BREAK_OLD_PAIR_CONFIRMATION,
  ESPNOW_MESSAGE_TYPE_REQUEST_MASTER_DEVICE_NAME,
  ESPNOW_MESSAGE_TYPE_SCREEN_UNPAIR_REQUEST,
  ESPNOW_MESSAGE_TYPE_AIR_CONDITIONER_UNPAIR_CONFIRMATION,
  ESPNOW_MESSAGE_TYPE_REQUEST_MASTER_DEVICE_NAME_CHANGE,
  ESPNOW_MESSAGE_TYPE_SINGLE_PHASE_METER_DATA,
  ESPNOW_MESSAGE_TYPE_SINGLE_PHASE_METER_SLABS_DATA,
  ESPNOW_MESSAGE_TYPE_THREE_PHASE_METER_DATA,
  ESPNOW_MESSAGE_DATA_SABRO_SCREEN_AUTHENTICATOR = 1184005015U,
  ESPNOW_MESSAGE_DATA_SABRO_AC_AUTHENTICATOR = 1184005016U,
  ESPNOW_MESSAGE_DATA_SABRO_SMALL_SCREEN_AUTHENTICATOR = 1184005014U,
};


enum screen_pairing_stages {
  ESPNOW_PAIRING_STAGE_NO_ACTIVITY,
  ESPNOW_PAIRING_STAGE_IS_PAIRED,
  ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY,
  ESPNOW_PAIRING_STAGE_PAIRING_REQUEST_SENT,
  ESPNOW_PAIRING_STAGE_PAIRING_REQUEST_ACKNOWLEDGED,
  ESPNOW_PAIRING_STAGE_PAIR_ACKNOWLEDGEMENT_REQUEST_RECIEVED,
  ESPNOW_PAIRING_STAGE_FORCE_PAIRING_REQUEST_SENT,
  ESPNOW_PAIRING_STAGE_FORCE_PAIRING_REQUEST_RECIEVED_FROM_AC,
  ESPNOW_PAIRING_STAGE_AC_PAIRED_TO_OTHER_DEVICE_ACKNOWLEDGEMENT_RECIEVED,
  ESPNOW_PAIRING_STAGE_SCREEN_REQUESTED_UNPAIRING,
  ESPNOW_PAIRING_STAGE_AC_CONFIRMED_UNPAIRING,
  ESPNOW_PAIRING_STAGE_REQUESTING_AVAILABILITY_BROADCAST_TIMEOUT = 5000,
  ESPNOW_PAIRING_STAGE_UNPAIRING_REQUESTED_TIMEOUT = 5000,
};

enum maximum_minimum_values {
    MIN_SET_TEMPERATURE = 16,
    MAX_SET_TEMPERATURE = 30,
  };

enum ac_sending_data_status_bits {
  SEND_AC_POWER_STATUS_BIT = 0,
  SEND_AC_SWING_STATUS_BIT,
  SEND_AC_ECO_MODE_STATUS,
  SEND_AC_RUNNING_MODE_BIT_0,
  SEND_AC_RUNNING_MODE_BIT_1,
  SEND_AC_RUNNING_MODE_BIT_2,
  SEND_AC_FAN_SPEED_BIT_0,
  SEND_AC_FAN_SPEED_BIT_1,
  SEND_AC_FAN_SPEED_BIT_2,
  SEND_AC_RUNNING_MODE_CLEAR = 0x1c7,
  SEND_AC_RUNNING_MODE_AUTO = 0x08,
  SEND_AC_RUNNING_MODE_COOL = 0x10,
  SEND_AC_RUNNING_MODE_FAN = 0x18,
  SEND_AC_RUNNING_MODE_HEAT = 0x20,
  SEND_AC_RUNNING_MODE_DRY = 0x28,
  SEND_AC_FAN_SPEED_CLEAR = 0x3f,
  SEND_AC_FAN_SPEED_LOW = 0x40,
  SEND_AC_FAN_SPEED_MEDIUM = 0x80,
  SEND_AC_FAN_SPEED_HIGH = 0xc0,
  SEND_AC_FAN_SPEED_AUTO = 0x100,
};

typedef struct sending_data_structure {
  uint8_t set_temperature;
  uint16_t status_bits;
} sending_data_t;

typedef struct ac_pairing_data_structure {
  char device_name[PAIRED_DEVICE_NAME_CHARACTER_COUNT];
  uint8_t is_paired;
  uint32_t authenticator;
} ac_pairing_data_t;

typedef struct pairing_request_structure {
  uint8_t message_type;
  uint32_t authenticator;
} pairing_request_t;



extern uint8_t available_connections;
extern uint8_t available_connections_macs[MAXIMUM_AVAILABLE_DEVICES_COUNT]
                                         [MAC_ADDRESS_ARRAY_SIZE];
extern uint8_t paired_mac[MAC_ADDRESS_ARRAY_SIZE];
extern char paired_device_name[PAIRED_DEVICE_NAME_CHARACTER_COUNT];
extern uint8_t incoming_mac[MAC_ADDRESS_ARRAY_SIZE];
//extern pairing_data_t incoming_pairing_data;
extern ac_pairing_data_t incoming_pairing_data;
extern uint8_t pairing_stage;
extern esp_now_peer_info_t peer_info;

extern bool is_paired;
extern bool is_pairing;
extern uint8_t broadcast_mac[MAC_ADDRESS_ARRAY_SIZE];
extern uint8_t pairing_mac[MAC_ADDRESS_ARRAY_SIZE];
extern uint8_t pairing_stage;
extern uint16_t unpairing_request_send_time;


extern realTime_data data;
extern sending_data_t send_data;
void esp_now_setup();
void request_paired_device_name();
void update_main_screen(realTime_data data);
void update_data_screen(realTime_data data);
void update_setting_screen();
void send_pair_reguest();
void unpair_device_action();
void force_unpair_close(lv_event_t *e);
void force_unpair(lv_event_t *e);
void force_pair_action(lv_event_t *e);
void force_pair_close_button_action(lv_event_t *e);
void action_send_data(lv_event_t *e);

// for serial printing
extern uint8_t last_sent_data[MAX_DATA_SIZE];
extern size_t last_sent_data_length;

#endif