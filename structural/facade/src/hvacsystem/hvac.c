#include "hvacsystem/hvac.h"
#include <stdio.h>
#include <stdlib.h>



void increaseTemp() {
	printf("Increasing temperature by 5.\n");
}

void decreaseTemp() {
	printf("Decreasing temperatur by -5.\n");
}

void hvacTurnOn() {
	printf("Turning on air condition.\n");
}

void hvacTurnOff() {
	printf("Turning off air condition.\n");
}

void autoSet() {
	printf("Get in auto setting mode.\n");
}

HvacSystem *createHvacSystem(){
	HvacSystem *hvacSystem = (HvacSystem*)malloc(sizeof(HvacSystem));
	hvacSystem->increaseTemp = increaseTemp;
	hvacSystem->decreaseTemp = decreaseTemp;
	hvacSystem->turnOn = hvacTurnOn;
	hvacSystem->turnOff = hvacTurnOff;
	hvacSystem->autoSet = autoSet;
	return hvacSystem;
}









