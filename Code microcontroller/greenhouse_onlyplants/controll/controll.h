#ifndef CONTROLL_H
#define CONTROLL_H
#include <stdint.h>
#include <MKL25Z4.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "switches.h"
#include "uart0.h"
#include "datanodered.h"
#include "fan.h"
#include "LEDS.h"
#include "HEATBED.h"
#include "battery.h"
#include "pump.h"
#include "bsp.h"
#include "aht21b.h"
#include "mistmodule.h" 
#include "lichtmodule.h"
#include "soilmoisture.h"
#include "wlevel.h"

extern volatile int targetval[5]; // 0 humidity, 1 soilmoisture, 2 illumination, 3 temperature
extern volatile int activeper[5]; // 0 humidity, 1 soilmoisture, 2 illumination, 3 temperature
extern volatile uint16_t activeval[5]; // 0 humidity, 1 soilmoisture, 2 illumination, 3 temperature
extern volatile int waterlvl;
extern volatile uint16_t sampletime;
extern volatile int scrollBack_timer;

extern volatile bool sw1_pressed_flag;
extern volatile bool sw2_pressed_flag;
extern volatile bool sw3_pressed_flag;
extern volatile bool sw4_pressed_flag;
extern volatile bool sw5_pressed_flag;
extern volatile bool v12_available;
extern volatile bool heatflag;

extern volatile bool sample_flag;
extern int sendcounter;
extern int errorcounter;
extern int pumpcounter;

void system_init(void);
void sample_init(uint16_t timeinMS);
void sample(void);
void regulate(void);
void sleep(void);

#endif
