#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commonchannnel.h"
#include "compress.h"
#include "crccheck.h"
#include "decorate.h"
#include "rawuartchannel.h"



int main() {
    printf("Starting communication channel simulation...\n\n");
    printf("1. Creating RawUartChannel (UART ID: 10)...\n");
    int id = 10;
    CommonChannel* common = createRawUartChannel(id);

    if (common == NULL) {
        return 1;
    }
    printf("2. Creating Compress decorator, wrapping UART channel...\n");

    CommonChannel* compress = createCompress(common);
    if (compress == NULL) {
        fprintf(stderr, "Failed to create Compress channel decorator. Exiting.\n");
        destroyRawUartChannel(common);
        return 1;
    }   
    printf("   Compress decorator created successfully.\n\n");
    printf("3. Creating CrcCheck decorator, wrapping Compress channel...\n");

    CommonChannel* crccheck = createCrcCheck(compress);

    if (crccheck == NULL) {
        fprintf(stderr, "Failed to create CRC channel decorator. Exiting.\n");
        destroyCompress(compress);
        destroyRawUartChannel(common);
        return 1;
    }
    printf("--- Sending Data ---\n");
    uint8_t original_data[] = {1, 1, 1, 2, 3, 3, 3, 3, 4, 5, 5, 5, 5, 5, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11};
    size_t original_length = sizeof(original_data);
    int send_status = crccheck->send(crccheck->instance, original_data, original_length);
    if (send_status == CODE_SUCCESS) {
        printf("Data sent successfully through the decorated channel.\n\n");
    } else {
        fprintf(stderr, "Error sending data: %d\n\n", send_status);
    }


    printf("--- Receiving Data ---\n");
    uint8_t received_data_buffer[256];
    size_t received_actual_length = 0;

    printf("Attempting to receive data from the decorated channel...\n");
    // Receive data from the top-most decorator (crcChannel)
    int receive_status = crccheck->received(crccheck->instance, received_data_buffer, sizeof(received_data_buffer), &received_actual_length);

    if (receive_status == CODE_SUCCESS) {
        printf("Data received successfully (actual length: %zu bytes): ", received_actual_length);
        for (size_t i = 0; i < received_actual_length; i++) {
            printf("%d ", received_data_buffer[i]);
        }
        printf("\n");

        // Verify if received data matches original data
        if (received_actual_length == original_length && memcmp(original_data, received_data_buffer, original_length) == 0) {
            printf("Verification: Received data matches original data. Success!\n");
        } else {
            printf("Verification: Received data MISMATCHES original data or length. Failure!\n");
            printf("Original length: %zu, Received length: %zu\n", original_length, received_actual_length);
        }
    } else {
        fprintf(stderr, "Error receiving data: %d\n", receive_status);
    }

    // --- Cleanup ---
    printf("\n--- Cleaning up allocated memory ---\n");
    // Destroy decorators in reverse order of creation (top-most first)
    destroyCrcCheck(crccheck); //
    printf("Destroyed CrcCheck decorator.\n");
    destroyCompress(compress); //
    printf("Destroyed Compress decorator.\n");
    destroyRawUartChannel(common); //
    printf("Destroyed RawUartChannel.\n");

    printf("\nSimulation finished.\n");

    return 0;

}