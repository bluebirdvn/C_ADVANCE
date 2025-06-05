#include "rawuartchannel.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int sendRawUart(void *instance, uint8_t *data, size_t length) {
    RawUartChannel *rawChannel = (RawUartChannel*)instance;
    printf("Send Raw data from uart channel: %d", rawChannel->uartChannelId);
    if (rawChannel == NULL) {
        return CODE_ERROR;
    }
    if (data == NULL) {
        if(length > 0) {
            return CODE_ERROR;
        } else return CODE_SUCCESS;
    }
    if (length == 0 && data != NULL) {
        return CODE_SENDFAIL;
    }
    if (length < sizeof(rawChannel->buffer_send)) {
        memcpy(rawChannel->buffer_send, data, length);
        rawChannel->size_buffer = length;
    } else {
        return CODE_ERROR;
    }
    return CODE_SUCCESS;
}


int receiveRawUart(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength) {
    RawUartChannel *rawChannel = (RawUartChannel*)instance;
    printf("Receive Raw data from uart channel: %d\n", rawChannel->uartChannelId);
    if (rawChannel == NULL) {
        *receivedlength = 0;
        return CODE_ERROR;
    }
    if (lengthbuffer == 0) {
        *receivedlength = 0;
        return CODE_SUCCESS;
    }
    size_t copyOfLength = (rawChannel->size_buffer > lengthbuffer) ? lengthbuffer : rawChannel->size_buffer;
    if (copyOfLength > 0) {
        memcpy(buffer, rawChannel->buffer_send, copyOfLength);
        rawChannel->size_buffer = 0;
    }
    *receivedlength = copyOfLength;
    return CODE_SUCCESS;
}

CommonChannel *createRawUartChannel(int uartChannelId) {
    CommonChannel *common = (CommonChannel*)malloc(sizeof(CommonChannel));
    if (!common) {
        return CODE_ERROR;
    }
    RawUartChannel *rawChannel = (RawUartChannel*)malloc(sizeof(RawUartChannel));
    if (!rawChannel) {
        return CODE_ERROR;
    }
    rawChannel->uartChannelId = uartChannelId;
    rawChannel->size_buffer = 0;
    common->instance = rawChannel;
    common->send = sendRawUart;
    common->received = receiveRawUart;
    return common;
}

void destroyRawUartChannel(CommonChannel *channel){
    if (channel) {
        printf("Destroying RawUartChannel (ID: %d)...\n", ((RawUartChannel*)channel->instance)->uartChannelId);
        if (channel->instance) {
            free(channel->instance); // Giải phóng RawUartChannel instance
            channel->instance = NULL; // Đặt con trỏ về NULL để tránh sử dụng sau khi giải phóng
        }
        free(channel); // Giải phóng CommonChannel wrapper
    }
}