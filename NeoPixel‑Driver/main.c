#include <stdio.h>
#include <stdint.h>
#include "led_driver.h"

#define NUM_PIXELS 10

void get_data(const uint32_t* buffer) {
    printf("Pixel 0: 0x%08X (expect 0x0000FF00)\n", buffer[0]);
    printf("Pixel 4: 0x%08X (expect 0x00FFFFFF)\n", buffer[4]);
    printf("Pixel 9: 0x%08X (expect 0x000000FF)\n", buffer[9]);
}


int main(void) {

    if (led_init(NUM_PIXELS) != 0) {
        printf("Initialize led RGB failed!");
        return 1;
    }

    const uint32_t* buffer = led_get_buffer();
    size_t size_of_buffer = led_get_pixel_count();

    if (!buffer || !size_of_buffer) {
        printf("Get buffer and size failed!");
        return 1;
    }

    for (int i = 0; i < size_of_buffer; ++i) {
        if (buffer[i] != 0) {
            printf("Buffer is NOT initialized to zero.");
            break;
        }
    }

    uint8_t R, G, B;
    int index;

    index = 0;
    R = 255;
    G = 0;
    B = 0;
    led_set_pixel_color(index, R, G, B);

    index = 9;
    R = 0;
    G = 255;
    B = 0;
    led_set_pixel_color(index, R, G, B);

    index = 4;
    R = 255;
    G = 255;
    B = 255;
    led_set_pixel_color(index, R, G, B);

    buffer = led_get_buffer();
    get_data(buffer);

    R = 0;
    G = 255;
    B = 0;
    led_fill(R, G, B);

    int all_green = 1;
    for (int i = 0; i < NUM_PIXELS; ++i) {
        if (buffer[i] != 0x00FF0000) {
            printf("Error in buffer[%d]", i);
            all_green = 0;
            break;
        }
    }

    printf("All pixel is Green: %s\n", all_green? "OK":"NOT");

    led_shutdown();

    return 0;
}

 


