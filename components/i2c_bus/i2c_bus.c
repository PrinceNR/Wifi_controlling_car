#include <stdio.h>
#include <stdbool.h>
#include "driver/i2c_master.h"
#include "i2c_bus.h"
#include "esp_err.h"





i2c_master_bus_handle_t i2c_bus_init(){

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    
    i2c_master_bus_handle_t bus_handle = NULL;


    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    return bus_handle;

}





