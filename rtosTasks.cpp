#include <Arduino.h>
#include "com_structs.h"
#include "widget_dec.h"
#include "rtosTasks.h"

TaskHandle_t updateTaskHandle;
volatile bool shouldSendMessage5 = false;

void initRTOS() {
    xTaskCreatePinnedToCore( updateTask,"UpdateTask",4096,NULL,1,&updateTaskHandle,1);
}

void updateTask(void *parameter) {
    while (true) {
        update_setting_screen();  // Call your update function
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay to prevent CPU hogging
    }
}