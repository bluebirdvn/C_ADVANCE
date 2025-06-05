#ifndef _COMPRESS_H
#define _COMPRESS_H

#include "decorate.h"
size_t rle_encode(const uint8_t* input, size_t len, uint8_t* output);
size_t rle_decode(const uint8_t* input, size_t len, uint8_t* output);

int compressSend(void *instace, uint8_t *data, size_t length);
int decompressReceive(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength);

CommonChannel *createCompress(CommonChannel *base);
void destroyCompress(CommonChannel *channel);
#endif