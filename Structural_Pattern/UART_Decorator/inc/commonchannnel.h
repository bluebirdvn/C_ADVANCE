#ifndef _COMMONCHANNEL_H
#define _COMMONCHANNEL_H
#include <stdint.h>
#include <stdio.h>

#define CODE_SUCCESS 0
#define CODE_SENDFAIL -1
#define CODE_RECEIVEFAIL -2
#define CODE_ERROR -3


typedef struct {
    void *instance;
    int (*send)(void *instance, uint8_t *data, size_t length);
    int (*received)(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength);
} CommonChannel;
#endif