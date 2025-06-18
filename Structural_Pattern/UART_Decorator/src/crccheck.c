#include "crccheck.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


uint8_t crc(const uint8_t *data, size_t length) {
    uint8_t crc_cal = 0;
    for (size_t i = 0; i < length; ++i) crc_cal ^= data[i];
    return crc_cal;
}

int CrcCheckSend(void *instance, uint8_t *data, size_t length) {
    DecorateChannel* CrcChannel = (DecorateChannel*)instance;
    CommonChannel *common = CrcChannel->base;

    if (CrcChannel == NULL || common == NULL) {
        return CODE_ERROR;
    }

    if (data == NULL) {
        return CODE_ERROR;
    }

    printf("CRC Check from channel\n");
    
    uint8_t crc_cal = crc(data, length);
    uint8_t *send_buffer_with_crc = (uint8_t*)malloc(length + 1);
    if (!send_buffer_with_crc) {
        printf("Failed to allocated buffer for sending with crc.\n");
        return CODE_ERROR;
    }

    memcpy(send_buffer_with_crc, data, length);
    send_buffer_with_crc[length] = crc_cal;
    int result = common->send(common->instance, send_buffer_with_crc, length+1);
    free(send_buffer_with_crc);
    return result;
}


int CrcCheckReceive(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength) {
    DecorateChannel *CrcChannel = (DecorateChannel*)instance;
    CommonChannel *common = CrcChannel->base;

    if (CrcChannel == NULL || common == NULL) {
        return CODE_ERROR;
    }
    if (receivedlength == NULL) {
        return CODE_ERROR;
    }
    printf("CRC check from receive.\n");
    uint8_t *buffer_receive = (uint8_t*)malloc(lengthbuffer + 1);
    if (!buffer_receive) {
        printf("Failed to allocate receive data with crc check.\n");
        *receivedlength = 0;
        return CODE_ERROR;
    }

    size_t actual_receive;
    size_t receive_status = common->received(common->instance, buffer_receive, lengthbuffer, &actual_receive);
    if (receive_status != CODE_SUCCESS) {
        printf("Receive Failed.\n");
        free(buffer_receive);
        *receivedlength = 0;
        return CODE_ERROR;
    }
    uint8_t crccal = buffer_receive[actual_receive-1];

    if (crccal != crc(buffer_receive, actual_receive-1)) {
        printf("Data error\n");
        free(buffer_receive);
        return CODE_ERROR;
    }
    printf("Data receive successfully\n");
    for (int i = 0; i < actual_receive - 1 ; ++i) {
        printf("%d ", buffer_receive[i]);
    }
    memcpy(buffer, buffer_receive, actual_receive-1);
    *receivedlength = actual_receive - 1;
    free(buffer_receive);
    return CODE_SUCCESS;
}



CommonChannel* createCrcCheck(CommonChannel *base) {
    DecorateChannel *CrcCheck = (DecorateChannel*)malloc(sizeof(DecorateChannel));
    CommonChannel *channel = (CommonChannel*)malloc(sizeof(CommonChannel));
    CrcCheck->base = base;
    channel->instance = CrcCheck;
    channel->send = CrcCheckSend;
    channel->received = CrcCheckReceive;
    return channel;
}
void destroyCrcCheck(CommonChannel *channel) {
    if (channel) {
        printf("Destroying CrcCheck decorator...\n");
        if (channel->instance) {
            DecorateChannel *decorator = (DecorateChannel*)channel->instance;
            // Quan trọng: Hàm destroy của decorator chỉ giải phóng chính nó.
            // Kênh cơ sở (decorator->base) sẽ được giải phóng bởi hàm destroy của chính nó
            // hoặc bởi hàm destroy tổng thể của chuỗi.
            free(decorator);
            channel->instance = NULL;
        }
        free(channel); // Giải phóng CommonChannel wrapper
    }
}