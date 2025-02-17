#ifndef RTOS_TASKS_H
#define RTOS_TASKS_H

#include <Arduino.h>

// Function declarations
void initRTOS();           // Initialize FreeRTOS tasks
void updateTask(void *parameter);  // Task function
void updateDataTask(void *parameter);
//void update();             // Update function to send messages

// Externally accessible flag to control sending Message 5
extern volatile bool shouldSendMessage5;

#endif