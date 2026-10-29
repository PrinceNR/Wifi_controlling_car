#include "stdio.h"
#include "string.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "oled.h"
#include "ssd1306.h"

i2c_master_dev_handle_t device_handle = NULL;


// Transmission 2 — Send display data
// 0x40 Data mode 0xFF 8 bits 0x00 8 bits 0xAA 8 bits
uint8_t data[128] = {0x40, 0xFF, 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA};
size_t bytes = sizeof(data);

// Transmission 3
// const uint8_t oled_data[] = { 0x00,  0xAE, 0X8D, 0X14, 0XA4, 0XAF};
const uint8_t oled_data[] = { 0x00, 0xAE, 0x8D, 0x14, 0xA4, 0xA6, 0x20, 0X02, 0XB0, 0X00, 0X10, 0XAF};
size_t oled_bytes = sizeof(oled_data);

uint8_t zeros[129] = {0x40};
// void zeros_fun(){
//     for (size_t i = 0; i < sizeof(zeros); i++)
//     {
//         zeros[i] = 0x00;
//     }
    
// }

void add_device(i2c_master_bus_handle_t bus_handle){
    
    i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x3C,
        .scl_speed_hz = 100000  //100khz
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &device_config, &device_handle));

}

void send_oled_cmd(void){

    ESP_ERROR_CHECK(i2c_master_transmit(
        device_handle,
        oled_data, oled_bytes,
        100 ));

}

void clear_oled(void){

    uint8_t zeros[129] = {0x40};

        for (uint8_t page = 0; page < 8; page++)
        {
        
        uint8_t cmds[4];
        cmds[0] = 0x00;              // Control byte: commands follow
        cmds[1] = 0xB0 | page;       
        cmds[2] = 0x00;              
        cmds[3] = 0x10;   

        ESP_ERROR_CHECK(i2c_master_transmit(
        device_handle,
        cmds, sizeof(cmds), 
        100 ));

        ESP_ERROR_CHECK(i2c_master_transmit(
        device_handle,
        zeros, sizeof(zeros),
        100 ));
        }
}



void send_oled_data(void){

    clear_oled();
    ESP_ERROR_CHECK(i2c_master_transmit(
        device_handle,
        &data[0], bytes,
        100 ));

}

