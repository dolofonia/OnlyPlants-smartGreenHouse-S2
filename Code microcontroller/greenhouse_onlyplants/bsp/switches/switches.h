#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdbool.h>
#include <MKL25Z4.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern volatile bool sw1_pressed_flag;
extern volatile bool sw2_pressed_flag;
extern volatile bool sw3_pressed_flag;
extern volatile bool sw4_pressed_flag;
extern volatile bool sw5_pressed_flag;


void sw_init(void);


#endif
