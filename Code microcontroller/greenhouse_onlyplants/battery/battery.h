#ifndef BATTERY_H
#define BATTERY_H
#include <stdint.h>
#include <MKL25Z4.h>
#include <math.h>
#include <stdbool.h>

bool power_available(void);
void bat_init(void);
double bat_sample(void);

	
#endif
