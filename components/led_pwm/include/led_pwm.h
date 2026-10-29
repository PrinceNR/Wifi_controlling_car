#pragma once

#include <stdint.h>

void led_pwm_init(void);
void led_pwm_set_duty(uint32_t duty);