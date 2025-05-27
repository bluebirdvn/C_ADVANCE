#include <stdio.h>
#include <stdlib.h>
#include "uartbuilder.h"

static UART_Builder* setBaudrate(UART_Builder* builder, uint32_t baudrate) {
    if (builder == NULL) {
        return NULL;
    }

    if (baudrate < 1200 || baudrate > 400000) {
        printf("Error: invalid baudrate.\n");
        return builder;
    }

    builder->config.baudrate = baudrate;
    return builder;
}

static UART_Builder* setParity(UART_Builder* builder, uint8_t parity) {
    if (builder == NULL) {
        return NULL;
    }

    if (parity > 2) {
        printf("Error: invalid parity.\n");
        return builder;
    }

    builder->config.parity = parity;
    return builder;
}

static UART_Builder* setStopBits(UART_Builder* builder, uint32_t stopBits) {
    if (builder == NULL) {
        return NULL;
    }

    if (stopBits < 1 || stopBits > 2) {
        printf("Error: invalid stopBits.\n");
        return builder;
    }

    builder->config.stopBits = stopBits;
    return builder;
}

static UART_Builder* setDataBits(UART_Builder* builder, uint32_t dataBits) {
    if (builder == NULL) {
        return NULL;
    }

    if (dataBits < 5 || dataBits > 9) {
        printf("Error: invalid dataBits.\n");
        return builder;
    }

    builder->config.dataBits = dataBits;
    return builder;
}

static UART_config_t* build(UART_Builder* builder) {
    if (builder == NULL) {
        return NULL;
    }
    return &builder->config;
}


UART_Builder UART_Builder_Init(void) {
    UART_Builder builder;

    builder.config.baudrate = 9600;
    builder.config.parity = 0;
    builder.config.dataBits = 8;
    builder.config.stopBits = 1;

    builder.setBaudrate = setBaudrate;
    builder.setDataBits = setDataBits;
    builder.setParity = setParity;
    builder.setStopBits = setStopBits;
    builder.build = build;
    return builder;
}
