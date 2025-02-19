// pwm.h
#ifndef PWM_H
#define PWM_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

void my_pwm_init(uint gpio);
void pwm_set_duty_cycle(uint gpio, uint16_t duty);
void pwm_disable(uint gpio);

#endif // PWM_H