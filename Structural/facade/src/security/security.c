#include "security/security.h"
#include <stdio.h>
#include <stdlib.h>



void turnAlarmOn() {
	printf("Turning on alarm...\n");
}

void turnAlarmOff() {
	printf("Turning off alarm...\n");
}

void activeSecurity() {
	printf("The security in active mode...\n");
}

void disableSecurity() {
	printf("The security in disable mode...\n");
}

SecuritySystem *createSecuritySystem() {
	SecuritySystem *securitySystem = (SecuritySystem*)malloc(sizeof(SecuritySystem));
	securitySystem->turnAlarmOn = turnAlarmOn;
	securitySystem->turnAlarmOff = turnAlarmOff;
	securitySystem->activeSecurity = activeSecurity;
	securitySystem->disableSecurity = disableSecurity;
	return securitySystem;
}





