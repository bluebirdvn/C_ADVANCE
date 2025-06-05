#ifndef _UARTBUILDER_H
#define _UARTBUILDER_H

#include <stdint.h>

#define BAUDRATE_9600 9600
#define BAUDRATE_19200 19200
#define BAUDRATE_38400 38400
#define BAUDRATE_57600 57600
#define BAUDRATE_115200 115200


typedef enum {
    DATA_BITS_5 = 5,
    DATA_BITS_6 = 6,
    DATA_BITS_7 = 7,
    DATA_BITS_8 = 8,
    DATA_BITS_9 = 9
} DataBits;


typedef enum {
    NONE = 0,
    EVEN = 1,
    ODD = 2
} Parity;


typedef enum {
    STOP_BIT_1 = 1,
    STOP_BIT_2 = 2
} StopBits;


typedef struct {
    uint32_t baudrate;
    Parity parity;
    StopBits stopBits;
    DataBits dataBits;
}UART_config_t;

struct UART_Builder;

typedef struct UART_Builder {
    UART_config_t config;

    struct UART_Builder* (*setBaudrate)(struct UART_Builder* builder, uint32_t baudrate);
    struct UART_Builder* (*setParity)(struct UART_Builder* builder, Parity parity);
    struct UART_Builder* (*setStopBits)(struct UART_Builder* builder, StopBits stopBits);
    struct UART_Builder* (*setDataBits)(struct UART_Builder* builder, DataBits dataBits);

    UART_config_t* (*build)(struct UART_Builder* builder);
} UART_Builder;

UART_Builder UART_Builder_Init(void);

#endif