#ifndef MAX6675_H_
#define MAX6675_H_

#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"
#include "esp_log.h"


typedef struct
{
    int MAX6675_SCK; // GPIO pin for MAX6675 serial clock (SCK)
    int MAX6675_CS; // GPIO pin for MAX6675 chip select (CS)
    int MAX6675_MISO; // GPIO pin for MAX6675 master input slave output (MISO)
    float TEMPERATURE_CALIBRATION_COEFFICIENT; // Temperature calibration coefficient for adjusting temperature readings
} MAX6675_structure;

#ifdef __cplusplus
extern "C"
{
#endif
/*
Max6675 initialization work, configuring pins, creating message queues
*/
void MAX6675_init(MAX6675_structure user_max6675_set);

/*
Reading max6675 data through shift
* @return 8-bit data
*/
char spi_read();

/*
Read current degrees Celsius
* @return  current temperature data
*/
double readCelsius();

/*
Read current Fahrenheit degrees
* @return  current temperature data
*/
double readFahrenheit();

double temperature_data_peek();

/*
Task to read the current temperature
*/
void Temperature_Task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif