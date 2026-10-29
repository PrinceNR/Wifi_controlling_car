#pragma once

#include "driver/i2c_master.h"

// void add_device( i2c_master_bus_handle_t bus_handle );

// void send_oled_cmd( void);

// void send_oled_data(void);

void add_oled_device(void);

void OLED_CLEAR(void);

void write_txt( const char *text);

void pressure_temp(float press, float temp);

