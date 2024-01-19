#include <stdio.h>
#include "unity.h"
#include "esp_log.h"
#include "max6675.h"

static const char* TAG = "MAX6675_TEST";

static MAX6675_structure max6675_test_set = {
// 第二代炸炉电子板 12段炸炉 max6675 GPIO
    .MAX6675_SCK = GPIO_NUM_36,
    .MAX6675_CS = GPIO_NUM_35,
    .MAX6675_MISO = GPIO_NUM_38,
    .TEMPERATURE_CALIBRATION_COEFFICIENT = 0.25
};

void max6675_test_init()
{
    // Initialize the MAX6675 sensor with test settings
    MAX6675_init(max6675_test_set);
    ESP_LOGI(TAG, "MAX6675 initialized");
}

void max6675_test_read_temperature()
{
    // Read temperature in Celsius and Fahrenheit
    double temp_celsius = readCelsius();
    double temp_fahrenheit = readFahrenheit();

    ESP_LOGI(TAG, "Temperature: %.2f °C, %.2f °F", temp_celsius, temp_fahrenheit);

    // Add checks to validate the temperature readings
    TEST_ASSERT_NOT_EQUAL(-100, temp_celsius); // Assuming -100 is error value
    // Further validation checks can be added based on known test conditions
}

TEST_CASE("MAX6675 Sensor Test", "[max6675][iot][sensor]")
{
    max6675_test_init();
    max6675_test_read_temperature();
}

void app_main(void)
{
    // print max6675 Test banner to serial
    /*
    *                         __    ________ _____   _______        _   
    *                        / /   / /____  | ____| |__   __|      | |  
    *  _ __ ___   __ ___  __/ /_  / /_   / /| |__      | | ___  ___| |_ 
    * | '_ ` _ \ / _` \ \/ / '_ \| '_ \ / / |___ \     | |/ _ \/ __| __|
    * | | | | | | (_| |>  <| (_) | (_) / /   ___) |    | |  __/\__ \ |_ 
    * |_| |_| |_|\__,_/_/\_\\___/ \___/_/   |____/     |_|\___||___/\__|                                                                  
    */
    printf("                         __    ________ _____   _______        _   \n");
    printf("                        / /   / /____  | ____| |__   __|      | |  \n");
    printf("  _ __ ___   __ ___  __/ /_  / /_   / /| |__      | | ___  ___| |_ \n");
    printf(" | '_ ` _ \\ / _` \\ \\/ / '_ \\| '_ \\ / / |___ \\     | |/ _ \\/ __| __|\n");
    printf(" | | | | | | (_| |>  <| (_) | (_) / /   ___) |    | |  __/\\__ \\ |_ \n");
    printf(" |_| |_| |_|\\__,_/_/\\_\\\\___/ \\___/_/   |____/     |_|\\___||___/\\__|\n");

    unity_run_menu();
}