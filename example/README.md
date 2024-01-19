# MAX6675 ESP-IDF Example

## Overview

This example demonstrates how to use the MAX6675 module with ESP-IDF (Espressif IoT Development Framework) on ESP32 or ESP32-S3 development boards. The MAX6675 is an external temperature sensor that communicates using the SPI (Serial Peripheral Interface) protocol.

The example initializes the MAX6675 module with user-configurable pins and a temperature calibration coefficient. It then creates a FreeRTOS task to continuously read the temperature from the MAX6675 sensor and output the temperature values.

## How to use example

### Hardware Required

To run this example, you will need the following hardware:

- One ESP development board (e.g., ESP32-WROVER Kit) or ESP core board (e.g., ESP32-DevKitC)
- MAX6675 module (external temperature sensor)

For more information about the MAX6675 module, you can refer to the [datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX6675.pdf).

#### Pin Assignment:

**Note:** The following pin assignments are used by default, but you can change them in the `menuconfig`.

| ESP32-S3 | MAX6675 |
| -------- | ------- |
| SCK      | SCK     |
| CS       | CS      |
| MISO     | MISO    |

Make sure to connect the corresponding pins between the ESP32-S3 board and the MAX6675 module.

### Configure the project
Before building the project, you need to configure the example according to your specific setup. Follow these steps:

1. Open the project configuration menu using the command `idf.py menuconfig`.
2. Navigate to the `MAX6675 Example Configuration` menu.

In this menu, you can configure the following options:

- **MAX6675 SCK pin:** Set the pin number for the SCK (serial clock) signal of the MAX6675. The default pin is 8.
- **MAX6675 CS pin:** Set the pin number for the CS (chip select) signal of the MAX6675. The default pin is 7.
- **MAX6675 MISO pin:** Set the pin number for the MISO (master in, slave out) signal of the MAX6675. The default pin is 6.
- **Temperature calibration coefficient:** Set the temperature calibration coefficient for the MAX6675. This coefficient is used to convert the raw sensor reading to actual temperature values. The default coefficient is 0.18.

Make any necessary changes to the configuration and save the settings.

### Build and Flash
To build, flash, and monitor the project, use the following command:

```bash
idf.py flash monitor -p PORT
```

Replace `PORT` with the serial port connected to your ESP development board.

While monitoring, the example will output the temperature readings obtained from the MAX6675 sensor.

To exit the serial monitor, type `Ctrl-]`.

## Example Output
The example will display the temperature readings obtained from the MAX6675 module. The output will vary depending on the actual temperature measurements.

```bash
I (timestamp): MAX6675: 26 °C
I (timestamp): MAX6675: 25 °C
I (timestamp): MAX6675: 24 °C
I (timestamp): MAX6675: 26 °C
I (timestamp): MAX6675: 25 °C
I (timestamp): MAX6675: 26 °C
I (timestamp): MAX6675: 27 °C
I (timestamp): MAX6675: 25 °C
```

## Troubleshooting
If you encounter any issues or have technical queries related to this example, please open an [issue](https://github.com/BrandonElectronic/max6675/issues) on the GitHub repository. We will respond to your inquiries as soon as possible.