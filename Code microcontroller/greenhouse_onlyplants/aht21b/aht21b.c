#include "aht21b.h"
uint8_t data[10] = {10};
	
	
bool aht21b_init(void){
	bool init_complete = false;
	uint8_t data1 = 0;
	//i2c1_init();
	delay_us(500000);
	i2c1_read(0x38<<1,0x71,&data1,1);
	if(data1 == 0x18){
		bool init_complete = true;
	}
	
	return init_complete;
}
void aht21b_getval(volatile int * temp,volatile int * hum){
	uint8_t datawrite[2] = {0x33,0x00};
	uint8_t dataread[8] = {0,0,0,0,0,0,0,0};
	uint32_t datareadtemp = 0;
	uint32_t datareadhum = 0;
	i2c1_write_adress(0x38<<1,0xAC,datawrite,2);
	delay_us(80000);
	i2c1_read_woutad(0x38<<1,dataread,8);
	datareadtemp = ((uint32_t)dataread[5]) | ((uint32_t)dataread[4]<<8) | (((uint32_t)dataread[3] & 0b00001111)<<16);
	datareadhum = ((uint32_t)dataread[1]<<12) | ((uint32_t)dataread[2]<<4) | (((uint32_t)dataread[3] & 0b11110000)>>4);
	*temp = ((int)(datareadtemp*200)/1048576)-50;
	*hum = ((int)(datareadhum*100)/1048576);
}

