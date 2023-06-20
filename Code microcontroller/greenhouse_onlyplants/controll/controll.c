#include "controll.h"

volatile int waterlvl = 2;
volatile int targetval[5] = {65,35,26,30,0};
volatile int activeper[5] = {11,11,11,11,0};
volatile uint16_t activeval[5] = {11,11,11,11,0};
volatile uint16_t sampletime = 500;
volatile int scrollBack_timer = 0;

volatile bool sw1_pressed_flag = false; 
volatile bool sw2_pressed_flag = false; 
volatile bool sw3_pressed_flag = false; 
volatile bool sw4_pressed_flag = false; 
volatile bool sw5_pressed_flag = false; 
int sendcounter = 0;
int errorcounter = 0;
int pumpcounter = 0;

volatile bool sample_flag = false;
volatile bool heatflag = false;
volatile bool v12_available = false;
bool fan_on = false;
bool led_on = false;
bool pump_on = false;
bool error = false;
int led_value = 0;
int count = 0;
char temp[10];

void system_init(void){
		ldr_init();
		bat_init();
		heatbed_init();
		node_init();
		sw_init();
		fan_init();
		leds_init();
		pump_init();
	  sample_init(sampletime);
		menu_init();
		aht21b_init();
		mistmodule_init();
		soil_init();
		wlevel_init();
		ssd1306_init();
		ssd1306_setorientation(1);
	
//	  while(1){
//			count++;
//			sprintf(temp,"%d",count);
//			ssd1306_putstring(0,0,temp);
//			ssd1306_update();
//			delay_us(1000000);
//			if(count == 5){
//				//pump_en(true);
//			}
//		}
	
}
void sample(void){
	aht21b_getval(&activeper[3],&activeper[0]);
	ldr_sample(&activeper[2]);
	soil_measure(&activeper[1]);
	waterlvl = wlevel_sample();
}
void regulate(void){
	if(waterlvl < 15){
		error = true;
	}
	if(activeper[3] < targetval[3]){ // temperature
		if(v12_available == true){
			heatbed_set(65535);
		}
		else{
			heatbed_set(0);
		}
	}
	else{
		heatbed_set(0);
		if(activeper[3] > targetval[3]){
			fan_on = true;
		}
	}
	if(activeper[2] < targetval[2]){ // illumination
			led_value = (targetval[2] - activeper[2])*10553;
			if(led_value > 65535){
				led_value = 65535;
			}
			led_on = true;
	}
	if(activeper[1] < targetval[1]){ // soil moisture
		pump_on = true;
	}
	if(activeper[0] < (targetval[0])){ // humidity
		if(v12_available == true){
			miston(true);
		}
		else{
			miston(false);
		}
	}
	else{
		miston(false);
	}
	//handle fan on flag
	if((v12_available == true) && (fan_on == true)){
		fan_set(65535,1);
	}
	else{
		fan_set(30000,1);
	}
	//handle led on flag and error warning		
	if((error == true) && (errorcounter == 4)){
		leds_set(0,'b');
		leds_set(65535,'r');	
	}
	else if(led_on == true){
		leds_set(led_value,'r');
		leds_set(led_value/2,'b');
	}
	else{
		leds_set(0,'b');
		leds_set(0,'r');
	}
	//handle pump on flag
	if((pump_on == true) && (10 < pumpcounter) && (pumpcounter < 20)){
		if(waterlvl > 15){
			pump_en(true);
		}
		else{
			pump_en(false);
		}
	}
	else if((pump_on == true)){
		pump_en(false);
	}
	else{
		pump_en(false);
		pumpcounter = 20;
	}
	
	fan_on = false;
	led_on = false;
	pump_on = false;
	error = false;
}


void sample_init(uint16_t timeinMS){
	uint16_t setval = 0;
	uint32_t temptime = timeinMS * 1000000;
	setval = (24000000/(1000/timeinMS))-1;
	
	SIM->SCGC6 |= SIM_SCGC6_PIT(1);
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;	
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(12e6-1);
	PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_CHN_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void sleep(void){
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
}


