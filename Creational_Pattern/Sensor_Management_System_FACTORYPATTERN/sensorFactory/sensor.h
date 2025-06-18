#ifndef _SENSOR_H
#define _SENSOR_H

typedef struct {
    void (*init)(void);
    float (*readData)(void);

}Sensor;

#endif