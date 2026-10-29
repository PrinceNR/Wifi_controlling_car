#include "led_pwm.h"
#include "esp_err.h"
#include "driver/ledc.h"


void led_pwm_init( void)
{

ledc_timer_config_t timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_10_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 5000,
    .clk_cfg = LEDC_AUTO_CLK,
};
ledc_channel_config_t channel_config = {
    .gpio_num = 18,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0,
};

ESP_ERROR_CHECK(ledc_timer_config(&timer_config));
ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

}

void led_pwm_set_duty(uint32_t duty)
{

        if (duty > 1023U)
    {
        duty = 1023U;
    }
ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));

// ESP_ERROR_CHECK (ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty , 0));

}