 
#ifndef _LICHTMODULE_H
#define _LICHTMODULE_H

#include <MKL25Z4.h>
#include <math.h>

void ldr_init(void);

void ldr_sample(volatile int * val);

#endif 



