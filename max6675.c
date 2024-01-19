#include "max6675.h"

MAX6675_structure max6675_set;

QueueHandle_t temperature_data_handle;

static const char* TAG = "MAX6675";

void MAX6675_init(MAX6675_structure user_max6675_set)
{
    max6675_set.MAX6675_SCK = user_max6675_set.MAX6675_SCK;
    max6675_set.MAX6675_CS = user_max6675_set.MAX6675_CS;
    max6675_set.MAX6675_MISO = user_max6675_set.MAX6675_MISO;
    max6675_set.TEMPERATURE_CALIBRATION_COEFFICIENT = user_max6675_set.TEMPERATURE_CALIBRATION_COEFFICIENT;
   
    gpio_config_t io_conf = {};

    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;

    // set as output mode 输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;

    // bit mask of the pins that you want to set,e.g.初始化引脚
    io_conf.pin_bit_mask = 1ULL << (gpio_num_t)max6675_set.MAX6675_SCK | 1ULL << (gpio_num_t)max6675_set.MAX6675_CS;

    // disable pull-down mode
    // 下拉失能
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    // disable pull-up mode
    // 上拉使能
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    // configure GPIO with the given settings
    gpio_config(&io_conf);

    // set as output mode 输入模式
    io_conf.mode = GPIO_MODE_INPUT;

    // bit mask of the pins that you want to set,e.g.初始化引脚
    io_conf.pin_bit_mask = 1ULL << (gpio_num_t)max6675_set.MAX6675_MISO;
    // 上拉使能
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    temperature_data_handle = xQueueCreate(1, sizeof(double)); 
}

char spi_read(void)
{
    int i;
    char d = 0;

    //read and write output of pin for every bit
    for (i = 7; i >= 0; i--)
    {
        //initialize reading
        gpio_set_level(max6675_set.MAX6675_SCK , 0);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if (gpio_get_level(max6675_set.MAX6675_MISO))
        {
            // set the bit to 0 no matter what
            d |= (1 << i);
        }
        // exit reading
        gpio_set_level(max6675_set.MAX6675_SCK , 1);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    return d;
}

double readCelsius(void)
{

    uint16_t v;

    gpio_set_level(max6675_set.MAX6675_CS, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);

    v = spi_read();
    v <<= 8;
    v |=spi_read();

    gpio_set_level(max6675_set.MAX6675_CS, 1);

    // note 3rd bit of thermocouple reserved for connection verification
    if (v & 0x4)
    {
        // uh oh, no thermocouple attached!
        return -100;
        // return -100;
    }

    v >>= 3;

    return v * max6675_set.TEMPERATURE_CALIBRATION_COEFFICIENT;
}

double readFahrenheit(void)
{
    return readCelsius() * 9.0 / 5.0 + 32;
}

double temperature_data_peek()
{
    double data;	
    	
    //peek into queue	
    xQueuePeek(temperature_data_handle,&data,1);	
    	
    return data;
}

void Temperature_Task(void *pvParameters)
{
    while (1)
    {
        double sum = 0;
        int count = 0;
        for (int i = 0; i < 5; i++) {
            double data = readCelsius();
            sum += data;
            count++;
            vTaskDelay(100 / portTICK_PERIOD_MS); // add a short delay between readings
        }
        double mean = sum / count;
        ESP_LOGI(TAG, "Mean temperature: %.0f °C", mean);
        xQueueOverwrite(temperature_data_handle, &mean);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}