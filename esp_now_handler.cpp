#include "com_structs.h"
#include "widget_dec.h"
#include "image_colours.h"


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
  memcpy(&data, incomingData, sizeof(data));
  Serial.println("Data Recieved");

  Serial.print("Temperatur:");
  Serial.println(data.temp);
  Serial.print("Coil Sensor:");
  Serial.println(data.coil_sensor); 
  update_main_screen(data);
  //update_data_screen(data);
  //data_received = true; 
}

void esp_now_setup(){
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK){
      Serial.println("ESP NOW FAILED!!");
      return;
    }
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  }
