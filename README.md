# DWIN DGUS HMI ESP-IDF Library

Official Arduino Library for DWIN DGUS T5L HMI Display.

Supporting Features till date:

- `getHWVersion()`
- `restartHMI()`
- `setPage()`
- `getPage()`
- `setBrightness()`
- `getBrightness()`
- `setVP()`
- `setText()`
- `beepHMI()`
- `listenEvents()`

## Installation: Add component to your project

Please use the component manager command add-dependency to add the `dwin-hmi-esp` to your project's dependency, during the CMake step the component will be downloaded automatically.

```shell
idf.py add-dependency "hayschan/dwin-hmi-esp"
```

## Usage

### Include DWIN Library

```cpp
#include "DWIN.h"
```

### Initialization

Construct and initialize the DWIN hmi object with:
- [UART port number](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html#_CPPv411uart_port_t)
- RX pin
- TX pin
- Baud rate

```cpp
// Create an instance of the DWIN class
DWIN hmi(UART_NUM_1, RX_PIN, TX_PIN, DGUS_BAUD);
```

### Define callback Function

```cpp
// Event Occurs when response comes from HMI
void onHMIEvent(std::string address, int lastByte, std::string message, std::string response)
{  
    ESP_LOGI("HMIEvent", "OnEvent : [ A : %s | D : %02X | M : %s | R : %s ]", address.c_str(), lastByte, message.c_str(), response.c_str());
    if (address == "1002") {
        // Take your custom action call
    }
}
```

### Setup

```cpp
hmi.echoEnabled(false);
hmi.hmiCallBack(onHMIEvent);
hmi.setPage(1);
```

### Endless loop

Listen to HMI Events.

```cpp
hmi.listen();
```