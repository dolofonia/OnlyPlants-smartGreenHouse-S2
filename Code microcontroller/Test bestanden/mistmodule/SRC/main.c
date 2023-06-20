#include <MKL25Z4.h>
#include <stdbool.h>
#include <stdio.h>
#include "mistmodule.h"
#include "uart0.h"
int main(void){
	mistmodule_init();
	uart0_init();
	char test2[5];
	while(1){
		miston(true);
		sprintf(test2,"%d\n\r",test);
		uart0_send_string(test2);
		delay_us(50000);
	}
}
