#include "bsp.h"

int menu_Level;

void menu_Level0();
void menu_Level1();
void menu_Level2();

void menu_Preset();

void cursor_Handler(int min, int max, int *cursor);

int vert_cursor1 = 0;
int vert_cursor2 = 0;
int vert_cursor3 = 0;
int cursor_Check = 0;

bool wentBack1 = false;

int changeValue = 50;

int maxValue;
int minValue;

volatile int humidity_Current 			= 0;
volatile int temperature_Current 		= 0;
volatile int soil_Moisture_Current	= 0;
volatile int illumination_Current 	= 0;
	
volatile int humidity_Target 				= 0;
volatile int temperature_Target 		= 0;
volatile int soil_Moisture_Target 	= 0;
volatile int illumination_Target 		= 0;


void menu_init() {
	ssd1306_init();
	
	// clearing the old values
	humidity_Current 			= 0;
	temperature_Current 		= 0;
	soil_Moisture_Current	= 0;
	illumination_Current 	= 0;
	
	humidity_Target 				= 0;
	temperature_Target 		= 0;
	soil_Moisture_Target 	= 0;
	illumination_Target 		= 0;
	
	menu_Level = 0;

	// Set initial LCD message
	ssd1306_setorientation(1);   
  ssd1306_clearscreen();
	//ssd1306_putstring(35, 25, "OnlyPlants");
	ssd1306_drawbitmap(logo);
  ssd1306_update();
	
	delay_us(2000000);
	
	ssd1306_clearscreen();
	ssd1306_drawbitmap(arrow_select1);
	ssd1306_putstring(10, 6, "HUMIDITY");
	ssd1306_putstring(10, 16, "TEMPERATURE");
	ssd1306_putstring(10, 26, "SOIL MOISTURE");
	ssd1306_putstring(10, 36, "ILLUMINATION");
	ssd1306_putstring(10, 46, "PRESETS");
  ssd1306_update();
	
	
}

void menu_Update() {
	switch(menu_Level) {
		case 0:
			menu_Level0();
		break;
		case 1:
			menu_Level1();
		break;
		case 2:
			menu_Level2();
		break;
		default:
			menu_Level0();
		break;
	}
}

void menu_Level0() {
	//cursor_Handler(0, 4, &vert_cursor);
	if (sw1_pressed_flag == true) {
		if (vert_cursor1 > 0) {
			vert_cursor1--;
			sw1_pressed_flag = false;
		} else {
			sw1_pressed_flag = false;
		}
	}
	if (sw4_pressed_flag == true) {
		if (vert_cursor1 < 4) {
			vert_cursor1++;
			sw4_pressed_flag = false;
		} else {
			sw4_pressed_flag = false;
		}
	}
	sw2_pressed_flag = false;
	sw5_pressed_flag = false;
	
	// checking if the user wants to go to the next level
	if (sw3_pressed_flag == true) {
		menu_Level = 1;
		sw3_pressed_flag = false;
	}
	
	// if there is a change in the cursor pos => update
	if(vert_cursor1 != cursor_Check || wentBack1 == true) {
		ssd1306_clearscreen();
		switch(vert_cursor1) {
			case 0:
				ssd1306_drawbitmap(arrow_select1);
				// setting the max and min for the changing of the target
				minValue = 0;
				maxValue = 100;
			break;
			case 1:
				ssd1306_drawbitmap(arrow_select2);
				minValue = 10;
				maxValue = 60;
			break;
			case 2:
				ssd1306_drawbitmap(arrow_select3);
				minValue = 0;
				maxValue = 100;
			break;
			case 3:
				ssd1306_drawbitmap(arrow_select4);
				minValue = 0;
				maxValue = 100;
			break;
			case 4:
				ssd1306_drawbitmap(arrow_select5);
			break;
			default:
				ssd1306_drawbitmap(arrow_select1);
				minValue = 0;
				maxValue = 100;
			break;
		}
		ssd1306_putstring(10, 6, "HUMIDITY");
		ssd1306_putstring(10, 16, "TEMPERATURE");
		ssd1306_putstring(10, 26, "SOIL MOISTURE");
		ssd1306_putstring(10, 36, "ILLUMINATION");
		ssd1306_putstring(10, 46, "PRESETS");
		
		ssd1306_update();
		
		cursor_Check = vert_cursor1;
		
		wentBack1 = false;
		
	}
}

void menu_Level1() {
	//depending on the selection made display the correct menu
	ssd1306_clearscreen();
	
	//checking if the user wants to go back
	if (sw2_pressed_flag == true) {
		menu_Level = 0;
		sw2_pressed_flag = false;
		wentBack1 = true;
	}
	
	//checking if the user wants to the next level
	if (sw3_pressed_flag == true && vert_cursor1 != 4) {
		menu_Level = 2;
		sw3_pressed_flag = false;
	}
	
	// always clearing the not used flags
	sw1_pressed_flag = false;
	sw5_pressed_flag = false;
	sw4_pressed_flag = false;
	
	// make a temp value for the displaying of an int
	char temp[4];
	
	switch(vert_cursor1) {
		case 0:
			ssd1306_drawbitmap(arrow_single);
			ssd1306_putstring(10, 6, "CHANGE TARGET");
		
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", humidity_Target);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", humidity_Current);
			ssd1306_putstring(110, 26, temp);
		break;
		case 1:
			ssd1306_drawbitmap(arrow_single);	
			ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", temperature_Target);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", temperature_Current);
			ssd1306_putstring(110, 26, temp);		
		break;
		case 2:
			ssd1306_drawbitmap(arrow_single);
		  ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", soil_Moisture_Target);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", soil_Moisture_Current);
			ssd1306_putstring(110, 26, temp);
		break;
		case 3:
			ssd1306_drawbitmap(arrow_single);
			ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", illumination_Target);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", illumination_Current);
			ssd1306_putstring(110, 26, temp);
		break;
		case 4:
			menu_Preset();
		break;
		default:
			ssd1306_drawbitmap(arrow_single);
			ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", humidity_Target);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", humidity_Current);
			ssd1306_putstring(110, 26, temp);
		break;
	}
	
	ssd1306_update();
	
}

void menu_Level2() {
	ssd1306_clearscreen();
	
	// checking the buttons
	if (sw1_pressed_flag == true) {
		if (vert_cursor2 > 0) {
			vert_cursor2--;
			sw1_pressed_flag = false;
		} else {
			sw1_pressed_flag = false;
			if (changeValue < maxValue) {
				changeValue++;
			}
		}
	}
	if (sw4_pressed_flag == true) {
		if (vert_cursor2 < 1) {
			vert_cursor2++;
			sw4_pressed_flag = false;
		} else {
			sw4_pressed_flag = false;
			if (changeValue > minValue) {
				changeValue--;
			}
		}
	}
	sw3_pressed_flag = false;
	
	// displaying the correct arrow depending on the vert_cursor
	if (vert_cursor2 < 1) {
		ssd1306_drawbitmap(arrow_topSelect);
	} else {
		ssd1306_drawbitmap(arrow_bottemSelect);
	}
	
	// displaying the changeValue
	char temp[4];
	sprintf(temp, "%d", changeValue);
	ssd1306_putstring(57, 25, temp);
	
	
	// updating the correct value depending on the cursor
	switch(vert_cursor1) {
		case 0:
			humidity_Target = changeValue;
		break;
		case 1:
			temperature_Target = changeValue;
		break;
		case 2:
			soil_Moisture_Target = changeValue;
		break;
		case 3:
			illumination_Target = changeValue;
		break;
	}
	
	//check if the user is ready
	if (sw5_pressed_flag == true) {
		menu_Level = 0;
		sw5_pressed_flag = false;
		wentBack1 = true;	
		changeValue = 50;
	}
	
	//chect if the user wants to go back
	if (sw2_pressed_flag == true) {
		menu_Level = 1;
		sw2_pressed_flag = false;
		changeValue = 50;
	}
	
	
	ssd1306_update();
	
	
}

void menu_Preset() {
	
	
	// handeling the user input
	if (sw1_pressed_flag == true) {
		if (vert_cursor3 > 0) {
			vert_cursor1--;
			sw1_pressed_flag = false;
		} else {
			sw1_pressed_flag = false;
		}
	}
	if (sw4_pressed_flag == true) {
		if (vert_cursor3 < 4) {
			vert_cursor1++;
			sw4_pressed_flag = false;
		} else {
			sw4_pressed_flag = false;
		}
	}
	sw2_pressed_flag = false;
	
	if(vert_cursor1 != cursor_Check) {
		ssd1306_clearscreen();
		switch(vert_cursor3) {
			case 0:
				ssd1306_drawbitmap(arrow_select1);
			break;
			case 1:
				ssd1306_drawbitmap(arrow_select2);
			break;
			case 2:
				ssd1306_drawbitmap(arrow_select3);
			break;
			case 3:
				ssd1306_drawbitmap(arrow_select4);
			break;
			case 4:
				ssd1306_drawbitmap(arrow_select5);
			break;
			default:
				ssd1306_drawbitmap(arrow_select1);
			break;
		}
		ssd1306_putstring(10, 6,  "PRESET1");
		ssd1306_putstring(10, 16, "PRESET2");
		ssd1306_putstring(10, 26, "PRESET3");
		ssd1306_putstring(10, 36, "PRESET4");
		ssd1306_putstring(10, 46, "PRESET5");
		
		ssd1306_update();
		
		cursor_Check = vert_cursor3;
	}
}
