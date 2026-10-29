#include "stdio.h"
#include "oled.h"
#include "esp_err.h"
#include "esp_log.h"
#include "ssd1306.h"





ssd1306_handle_t oled_handle;

// char text[] = "hallow world";





void add_oled_device(){

    ssd1306_config_t oled_config = {
        .bus = SSD1306_I2C,
        .height = 64,
        .width = 128,
        .iface.i2c = {
            .addr = 0x3c,
            .port = I2C_NUM_0,
            .rst_gpio =GPIO_NUM_NC
            
        },
        
    };
    ESP_ERROR_CHECK( ssd1306_new_i2c( &oled_config, &oled_handle));
}

void OLED_CLEAR(){
    esp_err_t error = ssd1306_clear(oled_handle);

    if (error){
        ESP_LOGI("OLED", "OLED Clear function failed");
    }

}

void write_txt(const char *text){
    // const char *text = "hallo world";


    ESP_ERROR_CHECK(ssd1306_draw_text(oled_handle, 1, 1, text, true));

    ESP_ERROR_CHECK(ssd1306_display(oled_handle));

    ESP_LOGI("OLED", "TEXT DISPLAYED SUCCESSFULLY %s", text);


}

void pressure_temp(float pres, float temp){

    float pressure = pres;
    float temprature = temp;

    char text[64];
    snprintf( text, sizeof(text), "Live Pressure : %.2f hpa,  Live Temprature :%.2f" ,pressure ,temprature  );

    write_txt(text);
}
