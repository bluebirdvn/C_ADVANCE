#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 

#include "commonchannnel.h"
#include "compress.h"
#include "crccheck.h"
#include "decorate.h"
#include "rawuartchannel.h"


CommonChannel* setupCommunicationChannel(int uart_id, CommonChannel** p_raw_uart, CommonChannel** p_compress);

void cleanupCommunicationChannel(CommonChannel* crccheck, CommonChannel* compress, CommonChannel* raw_uart);

void sendAndReceiveData(CommonChannel* channel, const uint8_t* original_data, size_t original_length);


int main() {
    printf("Starting communication channel simulation...\n\n");

    CommonChannel* raw_uart_channel = NULL;
    CommonChannel* compress_channel = NULL;
    CommonChannel* crccheck_channel = NULL;

    // 1. Setup the communication channel stack
    crccheck_channel = setupCommunicationChannel(10, &raw_uart_channel, &compress_channel);
    

    // 2. Send and receive data through the decorated channel
    uint8_t original_data[] = {11, 45, 76, 77, 33, 76, 234, 255, 189, 111};
    size_t original_length = sizeof(original_data);
    sendAndReceiveData(crccheck_channel, original_data, original_length);

    // 3. Clean up allocated memory
    cleanupCommunicationChannel(crccheck_channel, compress_channel, raw_uart_channel);

    printf("\nSimulation finished.\n");

    return 0;
}

// --- Helper Function Implementations ---

/**
 * @brief Sets up the communication channel stack: RawUartChannel -> Compress -> CrcCheck.
 *
 * @param uart_id The ID for the RawUartChannel.
 * @param p_raw_uart Pointer to a CommonChannel* to store the created RawUartChannel.
 * @param p_compress Pointer to a CommonChannel* to store the created Compress decorator.
 * @return A pointer to the final CrcCheck decorated channel on success, or NULL on failure.
 * Error messages are printed internally.
 */
CommonChannel* setupCommunicationChannel(int uart_id, CommonChannel** p_raw_uart, CommonChannel** p_compress) {

    printf("1. Creating RawUartChannel (UART ID: %d)...\n", uart_id);

    *p_raw_uart = createRawUartChannel(uart_id);
    if (*p_raw_uart == NULL) {
        fprintf(stderr, "Failed to create RawUartChannel. Exiting.\n");
        return NULL;
    }
    printf("   RawUartChannel created successfully.\n\n");


    printf("2. Creating Compress decorator, wrapping UART channel...\n");
    *p_compress = createCompress(*p_raw_uart);
    if (*p_compress == NULL) {
        fprintf(stderr, "Failed to create Compress channel decorator. Exiting.\n");
        destroyRawUartChannel(*p_raw_uart); // Clean up already created channel
        *p_raw_uart = NULL; // Mark as NULL to prevent double free attempts
        return NULL;
    }
    printf("   Compress decorator created successfully.\n\n");

    printf("3. Creating CrcCheck decorator, wrapping Compress channel...\n");
    CommonChannel* crccheck = createCrcCheck(*p_compress);
    if (crccheck == NULL) {
        fprintf(stderr, "Failed to create CRC channel decorator. Exiting.\n");
        destroyCompress(*p_compress);    // Clean up already created channel
        destroyRawUartChannel(*p_raw_uart); // Clean up already created channel
        *p_compress = NULL; // Mark as NULL
        *p_raw_uart = NULL; // Mark as NULL
        return NULL;
    }
    printf("   CrcCheck decorator created successfully.\n\n");

    return crccheck;
}

/**
 * @brief Sends data through the decorated channel and attempts to receive it back,
 * then verifies if the received data matches the original.
 *
 * @param channel The CommonChannel instance to use for sending and receiving.
 * @param original_data Pointer to the array of original data bytes.
 * @param original_length The number of bytes in the original_data array.
 */
void sendAndReceiveData(CommonChannel* channel, const uint8_t* original_data, size_t original_length) {

    printf("--- Sending Data ---\n");

    int send_status = channel->send(channel->instance, original_data, original_length);
    
    if (send_status == CODE_SUCCESS) {
        printf("Data sent successfully through the decorated channel.\n\n");
    } else {
        fprintf(stderr, "Error sending data: %d\n\n", send_status);
    }

    printf("--- Receiving Data ---\n");
    uint8_t received_data_buffer[256]; // Assuming a max receive buffer size
    size_t received_actual_length = 0;

    printf("Attempting to receive data from the decorated channel...\n");
    int receive_status = channel->received(channel->instance, received_data_buffer, sizeof(received_data_buffer), &received_actual_length);

    if (receive_status == CODE_SUCCESS) {

        printf("Data received successfully (actual length: %zu bytes): \n", received_actual_length);
        for (size_t i = 0; i < received_actual_length; i++) {
            printf("%d ", received_data_buffer[i]);
        }
        printf("\n");


        if (received_actual_length == original_length && memcmp(original_data, received_data_buffer, original_length) == 0) {
            printf("Verification: Received data matches original data. Success!\n");
        } else {
            printf("Verification: Received data MISMATCHES original data or length. Failure!\n");
            printf("Original length: %zu, Received length: %zu\n", original_length, received_actual_length);
        }

    } else {

        fprintf(stderr, "Error receiving data: %d\n", receive_status);
    }
}

/**
 * @brief Cleans up all allocated communication channel instances.
 *
 * @param crccheck The CrcCheck decorated channel.
 * @param compress The Compress decorated channel.
 * @param raw_uart The RawUartChannel instance.
 */
void cleanupCommunicationChannel(CommonChannel* crccheck, CommonChannel* compress, CommonChannel* raw_uart) {
    printf("\n--- Cleaning up allocated memory ---\n");

    // Destroy in reverse order of creation
    if (crccheck != NULL) {
        destroyCrcCheck(crccheck);
        printf("Destroyed CrcCheck decorator.\n");
    }
    if (compress != NULL) {
        destroyCompress(compress);
        printf("Destroyed Compress decorator.\n");
    }
    if (raw_uart != NULL) {
        destroyRawUartChannel(raw_uart);
        printf("Destroyed RawUartChannel.\n");
    }
}