#include "max6675.h"

void app_main(void)
{
    // MAX6675 parameters
    MAX6675_structure user_max6675_set = {
        // Change the constants in sdkconfig or menuconfig
        .MAX6675_SCK = GPIO_NUM_9,
        .MAX6675_CS = GPIO_NUM_8,
        .MAX6675_MISO = GPIO_NUM_7,
        .TEMPERATURE_CALIBRATION_COEFFICIENT = 0.25
    };

    // MAX6675 initialization
    MAX6675_init(user_max6675_set);
   
    // FreeRTOS task
    xTaskCreate(Temperature_Task, "MAX6675 Temperature Task", 2 * 1024, NULL, 3, NULL);
}