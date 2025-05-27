#include <stdio.h>
#include <stdlib.h>
#include "uartbuiler.h"

int main() {
    UART_Builder uartbuilder = UART_Builder_Init();
    printf("Cấu hình UART Mặc định (sau khi Init):\n");
    printf("  Baud Rate (mặc định): %lu\n", uartbuilder.config.baudrate);
    printf("  Parity (mặc định): %u\n", uartbuilder.config.parity);
    printf("  Stop Bits (mặc định): %u\n", uartbuilder.config.stopBits);
    printf("  Data Bits (mặc định): %u\n\n", uartbuilder.config.dataBits);

    UART_config_t final_config = uartbuilder.setBaudrate(&uartbuilder, 11500)
    ->setDataBits(&uartbuilder, 8)
    ->setParity(&uartbuilder, 0)
    ->setStopBits(&uartbuilder, 1)
    ->build(&uartbuilder);

    if (final_config) {
        printf("Cấu hình UART Cuối cùng:\n");
        printf("  Baud Rate: %lu\n", final_config->baudrate);
        printf("  Parity: %u (0:None, 1:Odd, 2:Even)\n", final_config->parity);
        printf("  Stop Bits: %u\n", final_config->stopBits);
        printf("  Data Bits: %u\n", final_config->dataBits);
    } else {
        printf("Lỗi: Không thể build cấu hình UART.\n");
    
    }
}