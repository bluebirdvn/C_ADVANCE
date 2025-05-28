#include <stdio.h>
#include <stdlib.h>
#include "uartbuilder.h"

int main() {
    UART_Builder Auartbuilder = UART_Builder_Init();
    printf("Cấu hình UART Mặc định (sau khi Init):\n");
    printf("  Baud Rate (mặc định): %u\n", Auartbuilder.config.baudrate);
    printf("  Parity (mặc định): %u\n", Auartbuilder.config.parity);
    printf("  Stop Bits (mặc định): %u\n", Auartbuilder.config.stopBits);
    printf("  Data Bits (mặc định): %u\n\n", Auartbuilder.config.dataBits);

    UART_config_t* final_config = 
       Auartbuilder.setBaudrate(&Auartbuilder, BAUDRATE_9600)
               ->setDataBits(&Auartbuilder, DATA_BITS_8)
               ->setParity(&Auartbuilder, EVEN)
               ->setStopBits(&Auartbuilder, STOP_BIT_1)
               ->build(&Auartbuilder);;

    if (final_config->baudrate!=0) {
        printf("Cấu hình UART Cuối cùng:\n");
        printf("  Baud Rate: %u\n", final_config->baudrate);
        printf("  Parity: %u (0:None, 1:Odd, 2:Even)\n", final_config->parity);
        printf("  Stop Bits: %u\n", final_config->stopBits);
        printf("  Data Bits: %u\n", final_config->dataBits);
    } else {
        printf("Lỗi: Không thể build cấu hình UART.\n");
    
    }
}