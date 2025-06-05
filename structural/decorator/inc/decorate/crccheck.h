#ifndef _CRCCHECK_H
#define _CRCCHECK_H

#include "decorate.h"



uint8_t crc(const uint8_t *data, size_t length);
int CrcCheckSend(void *instace, uint8_t *data, size_t length);
int CrcCheckReceive(void *instance, uint8_t *buffer, size_t lengthbuffer, size_t *receivedlength);
CommonChannel* createCrcCheck(CommonChannel *base);

#endif