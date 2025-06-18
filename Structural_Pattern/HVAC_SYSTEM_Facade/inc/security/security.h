#ifndef _SECURITY_H
#define _SECURITY_H

typedef struct {
	void (*turnAlarmOn)(void);
	void (*turnAlarmOff)(void);
	void (*activeSecurity)(void);
	void (*disableSecurity)(void);
}SecuritySystem;

SecuritySystem *createSecuritySystem();

#endif
