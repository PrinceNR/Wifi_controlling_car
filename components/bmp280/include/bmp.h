#pragma once

#include "driver/i2c_master.h"

void add_bmp_device(i2c_master_bus_handle_t bus_handle);

void add_bmp(i2c_master_bus_handle_t bus_handle);

void calibration_reading(void);

void raw_temp( void );

void Pressure_calculation(void);

float *measurement_fun (void);

