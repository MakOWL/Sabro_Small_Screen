#ifndef COM_STRUCTS_H
#define COM_STRUCTS_H

#include <esp_now.h>
#include <WiFi.h>

enum {
  PAIRED_DEVICE_NAME_CHAR_COUNT = 20,
  SMALL_SCREEN_AUTHENTICATOR = 1184005014U,
  MAC_ADDRESS_ARRAY_SIZE = 6
};

typedef struct realTime_data{
  int32_t warning_code;

  uint8_t rtc_sec;
  uint8_t rtc_min;
  uint8_t rtc_hour;
  uint8_t rtc_day;
  uint8_t rtc_month;
  uint32_t rtc_year;

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
  uint8_t in_error;
  uint8_t out_error;
  uint32_t timer_on_sec;
  uint32_t timer_of_sec;
} realTime_data;

//extern bool data_received = false;
extern realTime_data data;
void esp_now_setup();
void update_main_screen(realTime_data data);
void update_data_screen(realTime_data data);

#endif