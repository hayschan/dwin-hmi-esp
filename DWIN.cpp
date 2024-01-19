// DWIN.cpp

#include "DWIN.h"

// Constructor
DWIN::DWIN(uart_port_t uart_num, uint8_t receivePin, uint8_t transmitPin, long baud) {
    uart_config_t uart_config = {
        .baud_rate = DWIN_DEFAULT_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Configure UART parameters and install UART driver
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, transmitPin, receivePin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);

    this->_uart_num = uart_num;
    this->_isConnected = false; // Set connected flag to false initially
}

// Enable or Disable Echo
void DWIN::echoEnabled(bool enabled) {
    _echo = enabled;
}

// Get Hardware Firmware Version of DWIN HMI
double DWIN::getHWVersion() {
    // Command to get hardware version
    uint8_t sendBuffer[] = {CMD_HEAD1, CMD_HEAD2, 0x04, CMD_READ, 0x00, 0x0F, 0x01};
    uart_write_bytes(_uart_num, (const char*) sendBuffer, sizeof(sendBuffer));
    vTaskDelay(10 / portTICK_PERIOD_MS);
    return readCMDLastByte();
}

// Restart DWIN HMI
void DWIN::restartHMI() {
    uint8_t sendBuffer[] = {CMD_HEAD1, CMD_HEAD2, 0x07, CMD_WRITE, 0x00, 0x04, 0x55, 0xaa, CMD_HEAD1, CMD_HEAD2};
    uart_write_bytes(_uart_num, (const char*) sendBuffer, sizeof(sendBuffer));
    vTaskDelay(10 / portTICK_PERIOD_MS);
    readDWIN();
}

void DWIN::setPage(uint8_t pageID) {
    uint8_t sendBuffer[] = {CMD_HEAD1, CMD_HEAD2, 0x07, CMD_WRITE, 0x00, 0x84, 0x5A, 0x01, 0x00, pageID};
    uart_write_bytes(_uart_num, (const char*) sendBuffer, sizeof(sendBuffer));
    std::string response = readDWIN(); // Read the response from DWIN, if necessary
}

// Listen For incoming callback  event from HMI
void DWIN::listen(){
    handle();
}

// Read data from DWIN
std::string DWIN::readDWIN() {
    uint8_t data[BUF_SIZE];
    std::string response;
    int length = 0;
    unsigned long startTime = esp_timer_get_time() / 1000;

    while((esp_timer_get_time() / 1000 - startTime < READ_TIMEOUT)) {
        length = uart_read_bytes(_uart_num, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if(length > 0) {
            for(int i = 0; i < length; i++) {
                char hex[5];
                sprintf(hex, " %02X", data[i]);
                response += hex;
            }
        }
    }
    if(_echo) {
        ESP_LOGI("DWIN", "Received: %s", response.c_str());
    }
    return response;
}

// Check and format hex values
std::string DWIN::checkHex(uint8_t currentNo) {
    char hex[5];
    sprintf(hex, "%02X", currentNo);
    return std::string(hex);
}

std::string DWIN::handle() {
    // Assuming 'readDWIN()' populates a buffer with the received data
    std::string response = readDWIN();

    // Process the response
    if (!response.empty()) {
        // Parse the response
        // This is where you'll need to decode the messages according to your protocol
        // For example, you might extract command codes, data values, etc.

        // Extracted dummy data (replace this with actual parsing logic)
        std::string address = "extracted_address"; // Placeholder, extract actual address from response
        int lastByte = 0xFF; // Placeholder, extract actual last byte from response
        std::string message = "extracted_message"; // Placeholder, extract actual message from response

        // If a valid callback is set, call it with the parsed data
        if (listenerCallback != nullptr) {
            listenerCallback(address, lastByte, message, response);
        }

        // You might want to return some status or processed data
        return "Processed"; // Placeholder, return actual processed data or status
    }

    // Return empty string or some status indicator if no data was received or processed
    return "";
}


// Read the last uint8_t of a command response
uint8_t DWIN::readCMDLastByte() {
    uint8_t data[BUF_SIZE];
    uint8_t lastByte = 0xFF;
    int length = 0;
    unsigned long startTime = esp_timer_get_time() / 1000;

    while((esp_timer_get_time() / 1000 - startTime < CMD_READ_TIMEOUT)) {
        length = uart_read_bytes(_uart_num, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if(length > 0) {
            lastByte = data[length - 1];
        }
    }
    return lastByte;
}

// Flush the UART buffer
void DWIN::flushSerial() {
    uart_flush(_uart_num);
}

void DWIN::hmiCallBack(hmiListener callBackFunction) {
    listenerCallback = callBackFunction;
}
