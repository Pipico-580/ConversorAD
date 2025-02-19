#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include <stdlib.h>
#include "lib/font.h" // Certifique-se de que o caminho está correto

// Definições de GPIOs
#define ADC_X 26
#define ADC_Y 27
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BUTTON_JOYSTICK 22
#define BUTTON_A 5
#define I2C_SDA 14
#define I2C_SCL 15

// Variáveis globais
bool leds_pwm_ativos = true;
bool led_verde_estado = false;
int borda_estado = 0;
int quadrado_x = 60;
int quadrado_y = 28;

i2c_inst_t *i2c = i2c0;
ssd1306_t display;

// Função de debounce
bool debounce(uint gpio) {
    sleep_ms(50);
    return !gpio_get(gpio);
}

// Alternar borda do display
void alternar_borda() {
    borda_estado = (borda_estado + 1) % 3;
    ssd1306_clear(&display);
    if (borda_estado == 1) {
        ssd1306_draw_rect(&display, 0, 0, 128, 64, 1);
    } else if (borda_estado == 2) {
        ssd1306_draw_rect(&display, 2, 2, 124, 60, 1);
    }
    ssd1306_show(&display);
}

// Interrupção do botão do joystick
void irq_button_joystick(uint gpio, uint32_t events) {
    if (debounce(gpio)) {
        led_verde_estado = !led_verde_estado;
        gpio_put(LED_GREEN, led_verde_estado);
        alternar_borda();
        printf("Botão Joystick pressionado! LED Verde: %d\n", led_verde_estado);
    }
}

// Interrupção do botão A
void irq_button_a(uint gpio, uint32_t events) {
    if (debounce(gpio)) {
        leds_pwm_ativos = !leds_pwm_ativos;
        if (!leds_pwm_ativos) {
            pwm_set_gpio_level(LED_RED, 0);
            pwm_set_gpio_level(LED_BLUE, 0);
        }
        printf("Botão A pressionado! PWM LEDs: %d\n", leds_pwm_ativos);
    }
}

// Configuração do PWM para LEDs
void pwm_setup(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_init(slice_num, &config, true);
}

int main() {
    stdio_init_all();
    
    // Configuração dos botões
    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK, GPIO_IN);
    gpio_pull_up(BUTTON_JOYSTICK);
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    
    // Configuração do LED Verde
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0);
    
    // Configuração do ADC
    adc_init();
    adc_gpio_init(ADC_X);
    adc_gpio_init(ADC_Y);
    
    // Configuração do PWM nos LEDs RGB
    pwm_setup(LED_RED);
    pwm_setup(LED_BLUE);
    
    // Inicialização do I2C e do display
    i2c_init(i2c, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    if (!ssd1306_init(&display, 128, 64, i2c)) {
        printf("Erro ao inicializar o display OLED!\n");
    }
    
    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_JOYSTICK, GPIO_IRQ_EDGE_FALL, true, &irq_button_joystick);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &irq_button_a);
    
    while (1) {
        adc_select_input(0);
        int x_value = adc_read() * 4;
        adc_select_input(1);
        int y_value = adc_read() * 4;
        
        if (leds_pwm_ativos) {
            int red_duty = (abs(x_value - 2048) * 255) / 2048;
            int blue_duty = (abs(y_value - 2048) * 255) / 2048;
            pwm_set_gpio_level(LED_RED, red_duty * red_duty / 255);
            pwm_set_gpio_level(LED_BLUE, blue_duty * blue_duty / 255);
        }
        
        // Mapeamento do quadrado no display
        quadrado_x = (x_value * (128 - 8)) / 4095;
        quadrado_y = (y_value * (64 - 8)) / 4095;
        
        ssd1306_clear(&display);
        if (borda_estado == 1) {
            ssd1306_draw_rect(&display, 0, 0, 128, 64, 1);
        } else if (borda_estado == 2) {
            ssd1306_draw_rect(&display, 2, 2, 124, 60, 1);
        }
        ssd1306_fill_rect(&display, quadrado_x, quadrado_y, 8, 8, 1);
        ssd1306_show(&display);
        
        printf("X: %d, Y: %d\n", x_value, y_value);
        sleep_ms(50);
    }
}