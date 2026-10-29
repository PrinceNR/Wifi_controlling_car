// #include <stdio.h>
// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_err.h"
// #include "esp_adc/adc_oneshot.h"
// #include "driver/ledc.h"

// void app_main(void)
// {

//     //measure the potentiometer’s voltage using ADC, then use that reading to control LED brightness using PWM.


//     gpio_reset_pin(GPIO_NUM_18);
//     gpio_reset_pin(GPIO_NUM_2);

//     gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
//     gpio_set_level(GPIO_NUM_2, 0);
//     gpio_set_direction(GPIO_NUM_18, GPIO_MODE_INPUT);

//     gpio_set_pull_mode(GPIO_NUM_18, GPIO_PULLUP_ONLY);
    

//     while(1)
//     {
//         if (gpio_get_level(GPIO_NUM_18) == 0)
//     {
//         gpio_set_level(GPIO_NUM_2, 1);
//     }
//     else
//     {
//         gpio_set_level(GPIO_NUM_2, 0);
      
//     }
//         vTaskDelay(pdMS_TO_TICKS(20));
    
//     }
// }


#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "potentiometer.h"
#include "led_pwm.h"
#include "i2c_bus.h"
#include "oled.h"
#include "bmp.h"

void app_main(void)
{
    potentiometer_init();
    led_pwm_init();

    i2c_master_bus_handle_t bus = i2c_bus_init();

    ESP_ERROR_CHECK( i2c_master_probe( 
        bus,
        0x3c,
        100
    ));

    ESP_LOGI("OLED", "OLED detected at address 0x3c");

    add_oled_device();
    OLED_CLEAR();

    char text[] = "hallo world";
    write_txt(text);

    // send_oled_cmd();
    // send_oled_data();

    ESP_ERROR_CHECK( i2c_master_probe( 
        bus,
        0x76,
        100
    ));
    ESP_LOGI("OLED", "OLED detected at address 0x76");


    add_bmp_device(bus);
    add_bmp(bus);
    calibration_reading();
    raw_temp();
    Pressure_calculation();
    // measurement_fun();


    while (1)
    {
       float *values =  measurement_fun();

        if (values != NULL)
        {
            float temprature = values[0];
            float pressure = values[1];

            pressure_temp(temprature, pressure); 

        }
        
       
        uint16_t raw = potentiometer_read();

        uint32_t duty =
            ((uint32_t)raw * 1023U) / 4095U;

        led_pwm_set_duty(duty);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}