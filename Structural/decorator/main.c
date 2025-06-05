#include <stdio.h>
#include "commonchannnel.h"
#include "compress.h"
#include "crccheck.h"
#include "decorate.h"
#include "rawuartchannel.h"



int main() {
    int id = 10;
    CommonChannel* common = createRawUartChannel(id);
    CommonChannel* compress = createCompress(common);
    CommonChannel* crccheck = createCrcCheck(compress);

    
}