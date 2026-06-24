//---------------------wokvi code in c++
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>


// Handle for our Recursive Mutex
SemaphoreHandle_t xRecursiveMutex = NULL;


// Forward declarations of our Controller Tasks
void vTask(void *pvParameters);


// Simulated Nested Shared Resource Functions
void functionA();
void functionB();


void setup() {
    Serial.begin(115200);
    while(!Serial);


    // Create the Recursive Mutex
    xRecursiveMutex = xSemaphoreCreateRecursiveMutex();


    if (xRecursiveMutex != NULL) {
        // Create Tasks (High Priority and Low Priority)
        xTaskCreate(vTask, "TASK", 256, NULL, 1, NULL);
       
    } else {
        printf("Failed to create Recursive Mutex!\n");
    }
    vTaskStartScheduler();
}


void loop() {
    // Empty when running under FreeRTOS control
}


void functionB(){
  Serial.println("FunctionB trying to lock mutex\n");
    (xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY));
    Serial.println("FunctionB acquired mutex\n");
    vTaskDelay(pdMS_TO_TICKS(100));
    xSemaphoreGiveRecursive(xRecursiveMutex);
    Serial.println("FunctionB released mutex\n");
}
// --- Tasks ---


void functionA(){
   Serial.println("FunctionA trying to lock mutex\n");
    xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY);
     Serial.println("FunctionA acquired mutex\n");
    functionB();   // tries to lock the same mutex again
    xSemaphoreGiveRecursive(xRecursiveMutex);
     Serial.println("FunctionA released mutex\n\n");
}


void vTask(void *pvParameters) {
    for (;;) {
        functionA();
        vTaskDelay(pdMS_TO_TICKS(100));
}
}
