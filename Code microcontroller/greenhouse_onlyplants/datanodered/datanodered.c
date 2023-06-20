#include "datanodered.h"


void node_init(void){
	uart2_init();
}
void node_getint(volatile int * dataarray){
	char * token;
	char stringin[35] = "\r";	
	int i = 0;
	uart2_get_string(stringin);
	if(stringin[0] != '\r'){
		token = strtok(stringin, ",");
		while( token != NULL ){
			 dataarray[i] = atoi(token);
			 token = strtok(NULL, ",");
			 i++;
		}
	}
}
void node_sendstring(int target){
	char datasendarray[35];
	switch(target)
		{
		case 0:
			sprintf(datasendarray,"target,%d,%d,%d,%d\n",targetval[0],targetval[1],targetval[2],targetval[3]);
			uart2_send_string(datasendarray);
			break;
		case 1:
			if(sendcounter == 20){
			sprintf(datasendarray,"active,%d,%d,%d,%d,%d\n",activeper[0],activeper[1],activeper[2],activeper[3],waterlvl);
			uart2_send_string(datasendarray);
			sendcounter = 0;
			}
			break;
		default:break;	
	}
}

