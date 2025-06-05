#include "crccheck.h"
#include "rawuartchannel.h"

uint8_t crc(const uint8_t *data, size_t length) {
    uint8_t crc_cal = 0;
    for (size_t i = 0; i < length; ++i) crc_cal ^= data[i];
    return crc_cal;
}

int CrcCheckSend(void *instance, uint8_t *data, size_t length) {
    DecorateChannel* CrcChannel = (DecorateChannel*)instance;
    CommonChannel *comom = CrcChannel->base;

    printf("CRC Check from channel\n");
    uint8_t crc_cal = crc(data, length);
    memcpy(buffer_send, data, length);
    buffer_send[length] = crc_cal;
    return comom->send(comom->instance, buffer_send, length+1);
}


int CrcCheckReceive(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength) {
    DecorateChannel *CrcChannel = (DecorateChannel*)instance;
    CommonChannel *common = CrcChannel->base;
    printf("CRC check from receive.\n");
    uint8_t *buffer_receive = (uint8_t)malloc(lengthbuffer);
    size_t actual_receive;
    size_t len = common->received(common->instance, buffer_receive, lengthbuffer, &actual_receive);
    if (len < 2) {
        printf("Receive Failed.\n");
        free(buffer_receive);
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