#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Configurações do display
#define SSD1306_ADDR 0x3C  // Endereço padrão do SSD1306
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

typedef struct {
    i2c_inst_t *i2c_port;
    uint8_t address;
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
} ssd1306_t;

// Inicializa o display SSD1306
bool ssd1306_init(ssd1306_t *display, uint8_t width, uint8_t height, i2c_inst_t *i2c_port);

// Limpa o buffer do display
void ssd1306_clear(ssd1306_t *display);

// Atualiza a tela com os dados do buffer
void ssd1306_show(ssd1306_t *display);

// Desenha um pixel no buffer
void ssd1306_draw_pixel(ssd1306_t *display, int x, int y, bool color);

// Preenche um retângulo com uma cor
void ssd1306_fill_rect(ssd1306_t *display, int x, int y, int w, int h, bool color);

// Desenha um retângulo vazio
void ssd1306_draw_rect(ssd1306_t *display, int x, int y, int w, int h, bool color);

#endif // SSD1306_H