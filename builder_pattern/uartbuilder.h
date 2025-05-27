#ifndef _UARTBUILDER_H
#define _UARTBUILDER_H

#include <stdint.h>

typedef struct {
    uint32_t baudrate;
    uint8_t parity;
    uint8_t stopBits;
    uint8_t dataBits;
}UART_config_t;

struct UART_Builder;

typedef struct UART_Builder {
    UART_config_t config;

    struct UART_Builder* (*setBaudrate)(struct UART_Builder* builder, uint32_t baudrate);
    struct UART_Builder* (*setParity)(struct UART_Builder* builder, uint8_t parity);
    struct UART_Builder* (*setStopBits)(struct UART_Builder* builder, uint32_t stopBits);
    struct UART_Builder* (*setDataBits)(struct UART_Builder* builder, uint32_t dataBits);

    UART_config_t* (*build)(struct UART_Builder* builder);
} UART_Builder;

UART_Builder UART_Builder_Init(void);

#endif