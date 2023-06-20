#ifndef PUMP_H
#define PUMP_H
#include <stdint.h>
#include <MKL25Z4.h>
#include <math.h>
#include <stdbool.h>
#include "controll.h"

void pump_init(void);
void pump_en(bool state);

	
#endif
