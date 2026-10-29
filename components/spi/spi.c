// #include "stdio.h"
// #include "string.h"
// #include "esp_err.h"
// #include "esp_log.h"
// #include "driver/spi_master.h"


// static spi_host_device_t host_id = SPI2_HOST;

// static const spi_bus_config_t bus_config = {
//     .mosi_io_num = 23,
//     .miso_io_num = 19,
//     .sclk_io_num = 18,
//     .quadwp_io_num = -1,
//     .quadhd_io_num = -1,
//     .max_transfer_sz = 4096
// };

// static spi_dma_chan_t dma_channel = SPI_DMA_CH_AUTO;

// const spi_device_interface_config_t device_config ={
//    .command_bits = ?,
//    .address_bits =?,
//    .dummy_bits = ?,
//    .mode = ?,
//    .clock_source =?,
//    .duty_cycle_pos = ?,
//    .cs_ena_pretrans = ?,
//    .cs_ena_posttrans = ?,
//    .clock_speed_hz = ?,
//    .input_delay_ns = ?,
//    .sample_point = ?,
//    .spics_io_num = ?,
//    .flags = ?,
//    .queue_size = ?,
//    .pre_cb = ?,
//    .post_cb = ?
   
// };
// spi_device_handle_t device_handle;

// static spi_transaction_t trans_desc = {
//    .flags = ?,
//    .cmd = ?,
//    .addr = ?,
//    .length = ?,
//    .rxlength = ?,
//    .override_freq_hz = ?,
//    .user  = ?,
//    .tx_buffer = ?,
//    .tx_data = ?,
//    .rx_buffer = ?,
//    .rx_data = ?
// } ;



// void spi_bus_initialization(void){

//    ESP_ERROR_CHECK(spi_bus_initialize( host_id, &bus_config, dma_chanel) );

//    ESP_LOGI("SPI", "SPI BUS INITIALIZED SUCCESSFULLY");

// };

// void add_device(void){
//    ESP_ERROR_CHECK(spi_bus_add_device( host_id, &device_config, &device_handle))
//    ESP_LOGI("SPI", "SPI DEVICE ADDED SUCCESSFULLY");
// };

// void device_transmit(){
//    ESP_ERROR_CHECK(spi_device_transmit( device_handle, &trans_desc  ));
// };
