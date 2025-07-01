#include <stdint.h>
#include <string.h>
#include "led_driver.h"


static uint32_t* led_buffer = NULL;
static size_t led_num_pixels = 0;


int led_init(size_t num_pixels) {
    if (num_pixels == 0) {
        return -1;
    }

    if (led_buffer != NULL) {
        free(led_buffer);
        led_buffer = NULL;
    }

    led_buffer = (uint32_t*)calloc(num_pixels, sizeof(uint32_t));

    if (!led_buffer) {
        led_num_pixels = 0;
        return -1;
    }

    led_num_pixels = num_pixels;
    return 0;

}

void led_shutdown() {
    if (led_buffer) {
        free(led_buffer);
        led_buffer = NULL;

        led_num_pixels = 0;
    }
}

void led_set_pixel_color(size_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!led_buffer || index > led_num_pixels) {
        return;
    }

    uint32_t color = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;

    led_buffer[index] = color;
    

}

void led_fill(uint8_t r, uint8_t g, uint8_t b){
    if (!led_buffer) {
        return;
    }

    uint32_t color = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;

    for (int i = 0; i < led_num_pixels; ++i) {
        led_buffer[i] = color;
    }

}

void led_clear() {
    led_fill(0, 0, 0);
}

const uint32_t* led_get_buffer() {
    return led_buffer;
}

size_t led_get_pixel_count() {
    return led_num_pixels;
}