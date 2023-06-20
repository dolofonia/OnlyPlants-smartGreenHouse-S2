#ifndef DATANODERED_H
#define DATANODERED_H

#include "uart0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "controll.h"

void node_init(void);
void node_getint(volatile int * dataarray);
void node_sendstring(int target);


#endif
