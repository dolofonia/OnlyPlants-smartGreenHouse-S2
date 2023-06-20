#ifndef FAN_H
#define FAN_H
#include <stdint.h>
#include <MKL25Z4.h>

void fan_init(void);
void fan_set(uint16_t speed,int n);

#endif
