#include "stdint.h"

// PL111 LCD Controller Base Address and Registers
#define LCD_BASE        0x10020000
#define LCDTIM0         (LCD_BASE + 0x000)
#define LCDTIM1         (LCD_BASE + 0x004)
#define LCDTIM2         (LCD_BASE + 0x008)
#define LCDTIM3         (LCD_BASE + 0x00C)
#define LCDUPBASE       (LCD_BASE + 0x010)
#define LCDCTRL         (LCD_BASE + 0x018)

// Framebuffer Base Address
#define FRAMEBUFFER     0x80000000
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

// RGB565 Color Macros
#define RGB565(r, g, b) ((r << 11) | (g << 5) | (b))

// Function to Draw a Pixel
void draw_pixel(volatile uint16_t *framebuffer, int x, int y, uint16_t color) {
    framebuffer[y * SCREEN_WIDTH + x] = color;
}

// Main Function
void main(void) {
    // Initialize framebuffer
    volatile uint16_t *framebuffer = (volatile uint16_t *)FRAMEBUFFER;

    // Fill the screen with red (debug)
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            draw_pixel(framebuffer, x, y, RGB565(31, 0, 0));  // Red
        }
    }

    // Configure LCD Controller
    *(volatile uint32_t *)LCDUPBASE = FRAMEBUFFER;      // Set framebuffer address
    *(volatile uint32_t *)LCDTIM0 = 0x1313A4C4;        // Horizontal timing
    *(volatile uint32_t *)LCDTIM1 = 0x0505F657;        // Vertical timing
    *(volatile uint32_t *)LCDTIM2 = 0x071F1800;        // Clock and signal polarity
    *(volatile uint32_t *)LCDTIM3 = 0x00000001;        // Line end control
    *(volatile uint32_t *)LCDCTRL = 0x1829;            // Enable LCD, 16bpp, TFT

    while (1) {
        // Infinite loop to keep the display active
    }
}

