#include "stdio.h"
#include "string.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "bmp.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static i2c_master_dev_handle_t device_handle = NULL;

void add_bmp_device(i2c_master_bus_handle_t bus_handle){

    i2c_device_config_t device_config = {
        .dev_addr_length =  I2C_ADDR_BIT_LEN_7,
        .device_address =0x76,
        .scl_speed_hz = 100000,
        .scl_wait_us = 0,

    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device( bus_handle, &device_config, &device_handle));
}
uint8_t tx_data = 0xF7;
size_t tx_bytes = sizeof(tx_data);
uint8_t rx_data[6]; 
size_t rx_bytes = sizeof(rx_data);
const  uint8_t start_cmd[2] = {0xF4, 0x27};
double temprature;
static double pressure;
int32_t t_fine;
int32_t p_fine;
int32_t adc_P;
uint16_t dig_T1;
int16_t  dig_T2;
int16_t  dig_T3;

static void read_tempratur_calib(void){

    uint8_t calib_buf[6];
    uint8_t reg = 0x88; // Starting register address for dig_T1

    // Read 6 bytes starting from 0x88
    ESP_ERROR_CHECK(i2c_master_transmit_receive(
        device_handle, &reg, 1, calib_buf, 6, 100));

    dig_T1 = (uint16_t)((calib_buf[1] << 8) | calib_buf[0]);
    dig_T2 = (int16_t)((calib_buf[3] << 8) | calib_buf[2]);
    dig_T3 = (int16_t)((calib_buf[5] << 8) | calib_buf[4]);

    ESP_LOGI("BMP280", "Calib T1=%u T2=%d T3=%d", dig_T1, dig_T2, dig_T3);

}

void raw_temp( void){


    double var1, var2;

    ESP_ERROR_CHECK(i2c_master_transmit(device_handle, &start_cmd[0], 2, 100));
    vTaskDelay(pdMS_TO_TICKS(10));
    ESP_ERROR_CHECK(i2c_master_transmit_receive(device_handle, &tx_data, 1, &rx_data[0], rx_bytes, 100));

    ESP_LOGI("BMP280", "Printing the raw data : %d, %d, %d, %d, %d, %d\n", rx_data[0], rx_data[1], rx_data[2], rx_data[3], rx_data[4], rx_data[5]);
    ESP_LOGI("BMP280", "Raw: %02X %02X %02X %02X %02X %02X\n",
         rx_data[0], rx_data[1], rx_data[2],
         rx_data[3], rx_data[4], rx_data[5]);

    int32_t adc_T = ((int32_t)rx_data[3] << 12) | ((int32_t)rx_data[4] << 4)| (rx_data[5] >> 4);

    adc_P = ((int32_t)rx_data[0] << 12)  | ((int32_t)rx_data[1] << 4)| (rx_data[2] >> 4);

    var1 = (((double)adc_T) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    var2 = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0) *
        (((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);

    
    printf("adc_T = %ld\n", (long)adc_T);
    printf("adc_Raw_temprature = %.2f\n",(float)adc_T);
    ESP_LOGI("BMP280", "adc_T = %ld\n", (long)adc_T);
    ESP_LOGI("BMP280", "adc_P = %ld\n", (long)adc_P);

    t_fine = (int32_t)(var1 + var2);
    temprature = (var1 + var2) / 5120.0;

    printf("live temprature value : %.2f C\n", temprature);

}

uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

static void read_pressure_calib(){

    uint8_t buf[18];           // 9 coefficients × 2 bytes
    uint8_t reg = 0x8E;        // Start of pressure calibration

    ESP_ERROR_CHECK(i2c_master_transmit_receive(
        device_handle, &reg, 1, buf, sizeof(buf), 100));

    // Combine LSB + MSB for each coefficient
    dig_P1 = (uint16_t)((buf[1] << 8) | buf[0]);   // UNSIGNED
    dig_P2 = (int16_t)((buf[3] << 8) | buf[2]);    // signed
    dig_P3 = (int16_t)((buf[5] << 8) | buf[4]);
    dig_P4 = (int16_t)((buf[7] << 8) | buf[6]);
    dig_P5 = (int16_t)((buf[9] << 8) | buf[8]);
    dig_P6 = (int16_t)((buf[11] << 8) | buf[10]);
    dig_P7 = (int16_t)((buf[13] << 8) | buf[12]);
    dig_P8 = (int16_t)((buf[15] << 8) | buf[14]);
    dig_P9 = (int16_t)((buf[17] << 8) | buf[16]);

    ESP_LOGI("BMP280", "P1=%u P2=%d P3=%d P4=%d P5=%d P6=%d P7=%d P8=%d P9=%d",
             dig_P1, dig_P2, dig_P3, dig_P4, dig_P5,
             dig_P6, dig_P7, dig_P8, dig_P9);


}

void calibration_reading(void){
    read_tempratur_calib();
    read_pressure_calib();

}

void Pressure_calculation(void){

double var1, var2;

var1 = ((double)t_fine / 2.0) - 64000.0;
var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
var2 = var2 + var1 * ((double)dig_P5) * 2.0;
var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);

if (var1 == 0.0) {
    ESP_LOGE("PRESSURE", "dig_P1 is zero — calibration read failed");
    return; // Avoid division by zero
}

pressure = 1048576.0 - (double)adc_P;
pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
var1 = ((double)dig_P9) * pressure * pressure / 2147483648.0;
var2 = pressure * ((double)dig_P8) / 32768.0;
pressure = pressure + (var1 + var2 + ((double)dig_P7)) / 16.0;
// The result p is the pressure in Pa (Pascals)

ESP_LOGI( "PRESSURE", "Live pressure %.2f", pressure);

printf("live pressure VALUE : %.2f \n", pressure);


}





