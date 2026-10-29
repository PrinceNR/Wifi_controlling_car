#include "stdio.h"
#include "string.h"
#include "bmp280.h"
#include "esp_log.h"
#include "bmp.h"
#include "esp_err.h"


// Inside your init function:
bmp280_config_t dev_config = I2C_BMP280_CONFIG_DEFAULT;
bmp280_handle_t dev_handle;
float temperature;
float pressure;

void add_bmp(i2c_master_bus_handle_t bus_handle){

    ESP_ERROR_CHECK( bmp280_init(bus_handle, &dev_config, &dev_handle)); // bus_handle is your i2c_master_bus_handle_t 

    if (dev_handle == NULL)
    {
        ESP_LOGI("BMP", "BMP DEVICE NOT DETECTED/CONNECTION FAILED");
        return;
    }
    

    ESP_LOGI("BMP", "BMP DEVICE ADDED");

}

float *measurement_fun(void){

    static float values[2];

    esp_err_t result = bmp280_get_measurements(dev_handle, &temperature, &pressure);
    if (result == ESP_OK) {
        ESP_LOGI("BMP280", "Temp: %.2f C, Pressure: %.2f Pa", temperature, pressure/100);

        values[0] = temperature;
        values[1] = pressure/100;

        return values;

    };

    return NULL;
    

}


