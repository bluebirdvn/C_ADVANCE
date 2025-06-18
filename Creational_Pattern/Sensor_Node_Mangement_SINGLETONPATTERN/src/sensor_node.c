#include "sensor_node.h"
#include <stdlib.h>

int getData(SensorNode* node) {
    if (node == NULL) {
        return -1;
    }
    return (rand() % 1000) + node->id * 1000;
}
