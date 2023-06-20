 
#ifndef _LICHTMODULE_H
#define _LICHTMODULE_H

#include <MKL25Z4.h>
#include <math.h>
//#include "controll.h"

void ldr_init(void);

uint16_t ldr_sample(void);

#endif 



