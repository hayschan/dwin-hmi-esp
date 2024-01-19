// dwin_example_test.cpp

#include "DWIN.h"

#define ADDRESS_A     "1010"
#define ADDRESS_B     "1020"

#define DGUS_BAUD     115200
#define RX_PIN        16  // Set these according to your ESP32's pinout
#define TX_PIN        17

// Create an instance of the DWIN class
DWIN hmi(UART_NUM_1, RX_PIN, TX_PIN, DGUS_BAUD);

void hmi_listen_task(void * args)
{
    while (1) {
        hmi.listen(); // Listen HMI Events
        vTaskDelay(10 / portTICK_PERIOD_MS); // Add a small delay to allow other tasks to run
    }
}

// Event Occurs when response comes from HMI
void onHMIEvent(std::string address, int lastByte, std::string message, std::string response) {  
    ESP_LOGI("HMIEvent", "OnEvent : [ A : %s | D : %02X | M : %s | R : %s ]", address.c_str(), lastByte, message.c_str(), response.c_str());
    if (address == "1002") {
        // Take your custom action call
    }
}

extern "C" void app_main() {
    // Initialize logging
    esp_log_level_set("*", ESP_LOG_INFO);
    
    ESP_LOGI("HMI", "DWIN HMI ~ Hello World");
    hmi.echoEnabled(false);
    hmi.hmiCallBack(onHMIEvent);
    hmi.setPage(1);

    // Create a task for hmi.listen() to run in
    xTaskCreate(hmi_listen_task, "hmi_listen_task", 4096, NULL, 5, NULL);
}
