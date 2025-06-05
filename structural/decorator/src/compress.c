#include "compress.h"

size_t rle_encode(const uint8_t* input, size_t len, uint8_t* output) {
    size_t outIdx = 0;
    for (size_t i = 0; i < len;) {
        uint8_t count = 1;
        while (i + count < len && count < 255 && input[i] == input[i + count]) {
            ++count;
        }
        output[outIdx++] = count;
        output[outIdx++] = input[i];
        i += count;
    }
    return outIdx;
}
size_t rle_decode(const uint8_t* input, size_t len, uint8_t* output) {
    size_t outIdx = 0;
    for (size_t i = 0; i + 1 < len;) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        for (uint8_t j = 0; j < count; ++j) {
            output[outIdx++] = value;
        }
    }
    return outIdx;
}


int compressSend(void *instance, uint8_t *data, size_t length) {
    DecorateChannel *Compress = (DecorateChannel*)instance;
    CommonChannel *common = Compress->base;
    uint8_t* output_send = (uint8_t*)malloc(length*2);
    size_t index = rle_encode(data, length, output_send);
    int a= common->send(common->instance, output_send, index);
    free(output_send);
    return a;
}
int decompressReceive(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength) {
    DecorateChannel *Compress = (DecorateChannel*)instance;
    CommonChannel *common = Compress->base;
    uint8_t *buffer_re = (uint8_t*)malloc(lengthbuffer);
    size_t actuallen;
    size_t a = common->received(common->instance, buffer_re, lengthbuffer, actuallen);
    size_t index = rle_decode(buffer_re, actuallen, buffer);
    return index;
}


CommonChannel *createCompress(CommonChannel *base) {
    DecorateChannel *Compress = (DecorateChannel*)malloc(sizeof(DecorateChannel));
    CommonChannel *common = (CommonChannel*)malloc(sizeof(CommonChannel));
    Compress->base = base;
    common->instance = Compress;
    common->send = compressSend;
    common->received = decompressReceive;
    return common;
}