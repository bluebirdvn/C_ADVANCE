#ifndef _LIGHTING_H
#define _LIGHTING_H

typedef struct {
	void (*turnOn)(void);
	void (*turnOff)(void);
	void (*setBrightness)(void);
}LightingSystem;

LightingSystem *createLightingSystem();

#endif //_LIGHTING_H
