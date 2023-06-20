#ifndef LEDS_H
#define LEDS_H
#include <stdint.h>
#include <MKL25Z4.h>

void leds_init(void);
void leds_set(uint16_t speed,char n);

#endif
