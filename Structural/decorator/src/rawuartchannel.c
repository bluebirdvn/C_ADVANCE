#include "rawuartchannel.h"
#include "commonchannnel.h"


int sendRawUart(void *instance, uint8_t *data, size_t length) {
    RawUartChannel *rawChannel = (RawUartChannel*)instance;
    printf("Send Raw data from uart channel: %d", rawChannel->uartChannelId);
    if (rawChannel == NULL) {
        return CODE_ERROR;
    }
    if (data == NULL) {
        return CODE_ERROR;
    }
    if (length == 0 && data != NULL) {
        return CODE_SENDFAIL;
    }
    if (length < size_buffer) {
    memcpy(buffer_send, data, length);
    } else {
        return CODE_ERROR;
    }
    return CODE_SUCCESS;
}

int receiveRawUart(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength) {
    RawUartChannel *rawChannel = (RawUartChannel*)instance;
    printf("Receive Raw data from uart channel: %d", rawChannel->uartChannelId);
    if (rawChannel == NULL) {
        return CODE_ERROR;
    }
    if (lengthbuffer == 0) {
        return CODE_SUCCESS;
    }
    size_t copyOfLength = size_buffer>lengthbuffer? size_buffer:lengthbuffer;
    memcpy(buffer, buffer_send, copyOfLength);

    receivedlength = copyOfLength;
    return receivedlength;
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
    common->instance = rawChannel;
    common->send = sendRawUart;
    common->received = receiveRawUart;
    return common;
}


