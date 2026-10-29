#include "potentiometer.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"


adc_oneshot_unit_init_cfg_t adc_config = {
    .unit_id = ADC_UNIT_1,
};

adc_oneshot_unit_handle_t adc_handle;
adc_channel_t adc_channel = ADC_CHANNEL_6;   

const adc_oneshot_chan_cfg_t channel_config = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
};

void potentiometer_init(void) {

    esp_err_t status = adc_oneshot_new_unit(&adc_config, &adc_handle);

    ESP_ERROR_CHECK(status);


    status = adc_oneshot_config_channel(
        adc_handle,
        adc_channel,
        &channel_config
    );
    ESP_ERROR_CHECK(status);




}

uint16_t potentiometer_read(void) {
    int raw = 0;
    esp_err_t status = adc_oneshot_read(
                        adc_handle,
                        adc_channel,
                        &raw
                    );
ESP_ERROR_CHECK(status);

    return (uint16_t)raw;
}