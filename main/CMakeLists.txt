idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES potentiometer led_pwm freertos i2c_bus oled bmp280
                    )
