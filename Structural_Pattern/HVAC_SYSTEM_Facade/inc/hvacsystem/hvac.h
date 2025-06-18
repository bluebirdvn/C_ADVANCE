#ifndef _HVAC_H
#define _HVAC_H

typedef struct {
	void (*increaseTemp)(void);
	void (*decreaseTemp)(void);
	void (*turnOn)(void);
	void (*turnOff)(void);
	void (*autoSet)(void);
}HvacSystem;

HvacSystem *createHvacSystem();


#endif
