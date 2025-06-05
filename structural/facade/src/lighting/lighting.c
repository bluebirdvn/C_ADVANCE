#include "lighting/lighting.h"
#include <stdio.h>
#include <stdlib.h>
void turnOn() {
	printf("Set the light turning on.\n");
}

void turnOff() {
	printf("Set the light turning off.\n");
}

void setBrightness() {
	printf("Increase the brightness.\n");
}

LightingSystem *createLightingSystem() {
	LightingSystem *lightsystem = (LightingSystem*)malloc(sizeof(LightingSystem));
	lightsystem->turnOn = turnOn;
	lightsystem->turnOff = turnOff;
	lightsystem->setBrightness = setBrightness;
	return lightsystem;
}

