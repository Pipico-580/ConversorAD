// pwm.c
#include "pwm.h"

void my_pwm_init(uint gpio) {
    // Verifica se o GPIO é válido para PWM
    if (gpio >= NUM_BANK0_GPIOS) {
        return; // GPIO inválido
    }

    // Configura o GPIO para a função PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    // Obtém o número do "slice" (fatia) do PWM associado ao GPIO
    uint slice = pwm_gpio_to_slice_num(gpio);

    // Configura o divisor de clock (ajuste conforme necessário)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Define o divisor de clock como 4.0

    // Inicializa o PWM com a configuração
    pwm_init(slice, &config, true); // O terceiro parâmetro "true" inicia o PWM imediatamente

    // Habilita o PWM no slice correspondente
    pwm_set_enabled(slice, true);
}

void pwm_set_duty_cycle(uint gpio, uint16_t duty) {
    // Verifica se o GPIO é válido para PWM
    if (gpio >= NUM_BANK0_GPIOS) {
        return; // GPIO inválido
    }

    // Define o ciclo de trabalho (duty cycle) no GPIO
    pwm_set_gpio_level(gpio, duty);
}

void pwm_disable(uint gpio) {
    // Verifica se o GPIO é válido para PWM
    if (gpio >= NUM_BANK0_GPIOS) {
        return; // GPIO inválido
    }

    // Desabilita o PWM no slice correspondente
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_enabled(slice, false);
}