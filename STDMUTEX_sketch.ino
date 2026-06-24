//--------wokvi code in c++

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <string.h>

#define TOTAL_RESOURCES 3

SemaphoreHandle_t xGlobalMutex = NULL;

int resource[3] = {111, 222, 333};
const char *resourceowner[3] = {"Free", "Free", "Free"};

// Forward declarations for all 4 tasks
void StartTask1(void *argument); // Priority 1 (Low) - LPT
void StartTask2(void *argument); // Priority 2 (Med) - MPT
void StartTask3(void *argument); // Priority 3 (High) - HPT
void StartTask4(void *argument); // Priority 4 (Very High) - VHPT




void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }


 
xGlobalMutex = xSemaphoreCreateMutex();



 
  xTaskCreate(StartTask1, "Task1", 128, NULL, 1, NULL); // LPT
  xTaskCreate(StartTask2, "Task2", 128, NULL, 2, NULL); // MPT
  xTaskCreate(StartTask3, "Task3", 128, NULL, 3, NULL); // HPT
  xTaskCreate(StartTask4, "Task4", 128, NULL, 4, NULL); // VHPT




  vTaskStartScheduler();
}




void loop() {
  // Empty - FreeRTOS handles execution
}




int getresource(const char *taskname) {
  for(int i = 0; i < TOTAL_RESOURCES; i++) {
    if(strcmp(resourceowner[i], "Free") == 0) {
      resourceowner[i] = taskname;
      return i;
    }
  }
  return -1;
}




void printresourcetab() {
  Serial.println("\n RESOURCE TABLE");
  for(int i = 0; i < TOTAL_RESOURCES; i++) {
      Serial.print("resource ID: ");
      Serial.print(resource[i]);
      Serial.print("|Owner: ");
      Serial.println(resourceowner[i]);
  }
}




void releaseResource(int resID) {
    if (resID >= 0 && resID < TOTAL_RESOURCES) {
        resourceowner[resID] = "Free";
    }
}


void StartTask1(void *argument) { 
    int resID;
    for(;;) {
        Serial.println("LPT waiting for resource");
        if (xSemaphoreTake(xGlobalMutex, portMAX_DELAY) == pdTRUE) {
            resID = getresource("LPT");
            if (resID != -1) {
                Serial.print("LPT accessing resource ID: ");
                Serial.println(resource[resID]);
                printresourcetab();
            }
            vTaskDelay(pdMS_TO_TICKS(500)); 
            Serial.println("LPT finished using resource");
            releaseResource(resID);
            xSemaphoreGive(xGlobalMutex);
            
            vTaskDelay(pdMS_TO_TICKS(500)); 
        }
    }
}

void StartTask2(void *argument) {
    int resID;
    for(;;) {
        Serial.println("MPT waiting for resource");
        if (xSemaphoreTake(xGlobalMutex, portMAX_DELAY) == pdTRUE) {
            resID = getresource("MPT");
            if (resID != -1) {
                Serial.print("MPT accessing resource ID: ");
                Serial.println(resource[resID]);
                printresourcetab();
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            
            Serial.println("MPT finished using resource");
            releaseResource(resID);
            xSemaphoreGive(xGlobalMutex);
            
            vTaskDelay(pdMS_TO_TICKS(1500)); 
        }
    }
}

void StartTask3(void *argument) { 
    int resID;
    for(;;) {
        Serial.println("HPT waiting for resource");
        if (xSemaphoreTake(xGlobalMutex, portMAX_DELAY) == pdTRUE) {
            resID = getresource("HPT");
            if (resID != -1) {
                Serial.print("HPT accessing resource ID: ");
                Serial.println(resource[resID]);
                printresourcetab();
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            
            Serial.println("HPT finished using resource");
            releaseResource(resID);
            xSemaphoreGive(xGlobalMutex);
            
            vTaskDelay(pdMS_TO_TICKS(2500)); 
        }
    }
}

void StartTask4(void *argument) { 
    int resID;
    for(;;) {
        Serial.println("VHPT waiting for resource");
        if (xSemaphoreTake(xGlobalMutex, portMAX_DELAY) == pdTRUE) {
            resID = getresource("VHPT");
            if (resID != -1) {
                Serial.print("VHPT accessing resource ID: ");
                Serial.println(resource[resID]);
                printresourcetab();
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            
            Serial.println("VHPT finished using resource");
            releaseResource(resID);
            xSemaphoreGive(xGlobalMutex);
            
            vTaskDelay(pdMS_TO_TICKS(4000));
        }
    }
}
