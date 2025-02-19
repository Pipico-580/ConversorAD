#include "ssd1306.h"
#include <string.h>

// Comandos do SSD1306
#define SSD1306_CMD 0x00
#define SSD1306_DATA 0x40

// Comandos de inicialização do SSD1306
const uint8_t ssd1306_init_cmds[] = {
    0xAE, // Display OFF
    0xD5, 0x80, // Set display clock divide ratio
    0xA8, 0x3F, // Set multiplex ratio (altura 64px)
    0xD3, 0x00, // Set display offset
    0x40, // Set start line
    0x8D, 0x14, // Enable charge pump
    0x20, 0x00, // Memory addressing mode (horizontal)
    0xA1, // Set segment re-map
    0xC8, // Set COM output scan direction
    0xDA, 0x12, // Set COM pins hardware configuration
    0x81, 0xCF, // Set contrast
    0xD9, 0xF1, // Set pre-charge period
    0xDB, 0x40, // Set VCOMH deselect level
    0xA4, // Entire display ON (resume from RAM)
    0xA6, // Set normal display mode
    0xAF  // Display ON
};

// Envia comandos para o display
static void ssd1306_send_command(ssd1306_t *display, uint8_t cmd) {
    uint8_t buffer[2] = {SSD1306_CMD, cmd};
    i2c_write_blocking(display->i2c_port, display->address, buffer, 2, false);
}

// Inicializa o display SSD1306
bool ssd1306_init(ssd1306_t *display, uint8_t width, uint8_t height, i2c_inst_t *i2c_port) {
    display->i2c_port = i2c_port;
    display->address = SSD1306_ADDR;
    memset(display->buffer, 0, sizeof(display->buffer));

    // Enviar comandos de inicialização
    for (size_t i = 0; i < sizeof(ssd1306_init_cmds); i++) {
        ssd1306_send_command(display, ssd1306_init_cmds[i]);
    }

    return true;
}

// Atualiza a tela com o buffer
void ssd1306_show(ssd1306_t *display) {
    uint8_t buffer[SSD1306_WIDTH + 1];
    buffer[0] = SSD1306_DATA; // Prefixo de dados
    
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_send_command(display, 0xB0 + page); // Seleciona página
        ssd1306_send_command(display, 0x00);        // Coluna baixa
        ssd1306_send_command(display, 0x10);        // Coluna alta

        memcpy(buffer + 1, &display->buffer[page * SSD1306_WIDTH], SSD1306_WIDTH);
        i2c_write_blocking(display->i2c_port, display->address, buffer, SSD1306_WIDTH + 1, false);
    }
}

// Limpa o buffer do display
void ssd1306_clear(ssd1306_t *display) {
    memset(display->buffer, 0, sizeof(display->buffer));
}

// Desenha um pixel no buffer
void ssd1306_draw_pixel(ssd1306_t *display, int x, int y, bool color) {
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;

    uint16_t index = x + (y / 8) * SSD1306_WIDTH;
    if (color) {
        display->buffer[index] |= (1 << (y % 8));
    } else {
        display->buffer[index] &= ~(1 << (y % 8));
    }
}

// Preenche um retângulo com uma cor
void ssd1306_fill_rect(ssd1306_t *display, int x, int y, int w, int h, bool color) {
    for (int dx = 0; dx < w; dx++) {
        for (int dy = 0; dy < h; dy++) {
            ssd1306_draw_pixel(display, x + dx, y + dy, color);
        }
    }
}

// Desenha um retângulo vazio
void ssd1306_draw_rect(ssd1306_t *display, int x, int y, int w, int h, bool color) {
    for (int dx = 0; dx < w; dx++) {
        ssd1306_draw_pixel(display, x + dx, y, color);
        ssd1306_draw_pixel(display, x + dx, y + h - 1, color);
    }
    for (int dy = 0; dy < h; dy++) {
        ssd1306_draw_pixel(display, x, y + dy, color);
        ssd1306_draw_pixel(display, x + w - 1, y + dy, color);
    }
}