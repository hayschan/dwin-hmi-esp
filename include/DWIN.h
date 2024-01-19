// DWIN.h

#ifndef DWIN_H
#define DWIN_H

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define DWIN_DEFAULT_BAUD_RATE      115200
#define BUF_SIZE                    1024

#define CMD_HEAD1           0x5A
#define CMD_HEAD2           0xA5
#define CMD_WRITE           0x82
#define CMD_READ            0x83

#define READ_TIMEOUT        100  // Adjust these values as needed
#define CMD_READ_TIMEOUT    50

class DWIN {
public:
    // Constructor: Initialize UART for communication with DWIN display
    DWIN(uart_port_t uart_num, uint8_t receivePin, uint8_t transmitPin, long baud = DWIN_DEFAULT_BAUD_RATE);

    // PUBLIC Methods

    void echoEnabled(bool enabled);       // Enable or disable echo
    void listen();                        // Listen for Touch Events & Messages from HMI
    double getHWVersion();                // Get Hardware Firmware Version of DWIN HMI
    void restartHMI();                    // Restart DWIN HMI
    void setPage(uint8_t pageID);            // Set the active page on HMI
    uint8_t getPage();                       // Get the current active page ID
    void setBrightness(uint8_t brightness);  // Set LCD Brightness
    uint8_t getBrightness();                 // Get LCD Brightness
    void setText(long address, const char* textData); // Set text on a specific VP (Variable Position)
    void setVP(long address, uint8_t data);  // Suint8_t te data on a specific VP
    void beepHMI();                       // Activate HMI buzzer for a short period

    // Callback Function type
    typedef void (*hmiListener) (std::string address, int lastByte, std::string message, std::string response);

    // Set the callback function for HMI events
    void hmiCallBack(hmiListener callBackFunction);

private:
    uart_port_t _uart_num;  // UART port number
    bool _echo;             // Echo enabled status
    bool _isConnected;      // Connection status

    hmiListener listenerCallback; // Callback function for HMI events

    void init(uart_port_t uart_num); // Initialize DWIN communication
    std::string readDWIN();          // Read data from DWIN
    std::string handle();            // Handle incoming data from DWIN
    std::string checkHex(uint8_t currentNo); // Check and format hex values
    uint8_t readCMDLastByte();          // Read the lauint8_t te of a command response
    void flushSerial();              // Flush the UART buffer
};

#endif // DWIN_H
