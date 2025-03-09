#include "watch_dog.h"
#include <esp_task_wdt.h>
#include <Arduino.h>

void watchdog_init(){
  if(esp_task_wdt_deinit () != ESP_OK ){
    Serial.println("Error while watch dog deinitializing");
    return; 
  }
  esp_task_wdt_config_t wdt_config = {
      .timeout_ms = WATCHDOG_TIMEOUT_IN_SECONDS * 1000,
        .idle_core_mask = 1 << 0,
        .trigger_panic = true
    };

    esp_task_wdt_init(&wdt_config);
    esp_task_wdt_add(NULL);

}