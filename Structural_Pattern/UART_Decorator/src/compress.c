#include "compress.h"
#include <stdlib.h>
#include <string.h>

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
    if (input == NULL || output == NULL) {
        return CODE_ERROR;
    }
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
    
    if (Compress == NULL || common == NULL) {
        return CODE_ERROR;
    }
    if (data == NULL & length > 0) {
        return CODE_ERROR;
    }
    if (length == 0) return CODE_SUCCESS;


    uint8_t* output_send = (uint8_t*)malloc(length * 2);
    if (!output_send) {
        printf("Failed to allocate buffer for compress.\n");
        return CODE_ERROR;
    }

    size_t index = rle_encode(data, length, output_send);
    int result = common->send(common->instance, output_send, index);
    free(output_send);
    return result;
}


int decompressReceive(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength) {
    DecorateChannel *Compress = (DecorateChannel*)instance;
    CommonChannel *common = Compress->base;
    if (Compress == NULL | common == NULL) {
        return CODE_ERROR;
    }
    if (receivedlength == NULL) {
        return CODE_ERROR;
    }
    if (lengthbuffer == 0 ) {
        *receivedlength = 0;
        return CODE_SUCCESS;
    }
    uint8_t *buffer_re = (uint8_t*)malloc(lengthbuffer);
    if (!buffer_re) {
        printf("Failed to allocate buffer for decompress.\n");
        return CODE_ERROR;
    }
    size_t actual_received_compress_len;
    size_t received_status = common->received(common->instance, buffer_re, lengthbuffer, &actual_received_compress_len);
    if (received_status != CODE_SUCCESS) {
        free(buffer_re);
        *receivedlength = 0;
        return received_status;
    }

    size_t index = rle_decode(buffer_re, actual_received_compress_len, buffer);
    free(buffer_re);
    if (index > lengthbuffer) {
        printf("Error: decompress data exceeds buffer size.\n");
        *receivedlength = 0;
        return CODE_ERROR;
    }
    *receivedlength = index;

    return CODE_SUCCESS;
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
void destroyCompress(CommonChannel *channel) {
    if (channel) {
        printf("Destroying Compress decorator...\n");
        if (channel->instance) {
            DecorateChannel *decorator = (DecorateChannel*)channel->instance;
            // Tương tự, chỉ giải phóng decorator này. Base channel được xử lý riêng.
            free(decorator);
            channel->instance = NULL;
        }
        free(channel); // Giải phóng CommonChannel wrapper
    }
}