#include "bsp.h"
#include "controll.h"

#define changeReset 40

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



void menu_init() {
	ssd1306_init();
	
	// clearing the old values	
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
		case 3:
			menu_Preset();
		break;
		default:
			menu_Level0();
		break;
	}
	if (scrollBack_timer >= 40) {
		menu_Level = 0;
		wentBack1 = true;
		scrollBack_timer = 0;
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
		scrollBack_timer = 0;
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
		switch(vert_cursor1) {
			case 0:
				changeValue = targetval[0];
			break;
			case 1:
				changeValue = targetval[3];
			break;
			case 2:
				changeValue = targetval[1];
			break;
			case 3:
				changeValue = targetval[2];
			break;
			default:
				changeValue = changeReset;
			break;
		}
	}
	
	// always clearing the not used flags
	sw1_pressed_flag = false;
	sw5_pressed_flag = false;
	sw4_pressed_flag = false;
	
	// make a temp value for the displaying of an int
	char temp[4];
	
	switch(vert_cursor1) {
		case 0: // hum
			ssd1306_drawbitmap(arrow_single);
			ssd1306_putstring(10, 6, "CHANGE TARGET");
		
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", targetval[0]);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", activeper[0]);
			ssd1306_putstring(110, 26, temp);
		break;
		case 1: // temp
			ssd1306_drawbitmap(arrow_single);	
			ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", targetval[3]);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", activeper[3]);
			ssd1306_putstring(110, 26, temp);	
		
			// scroll message, note that the textcontent should always have 19 spaces in front and at the end to prevent read errors.
			char textContent[101] = "                   NOTE! a high temp is often not possible due to thermal losses.                    ";
			int ofset = 2 * scrollBack_timer;
			char disText[20];
			sprintf(disText, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", textContent[ofset], textContent[ofset + 1], textContent[ofset + 2], 
							textContent[ofset + 3], textContent[ofset + 4], textContent[ofset + 5], textContent[ofset + 6], textContent[ofset + 7], textContent[ofset + 8], 
							textContent[ofset + 9], textContent[ofset + 10], textContent[ofset + 11], textContent[ofset + 12], textContent[ofset + 13], textContent[ofset + 14], 
							textContent[ofset + 15], textContent[ofset + 16], textContent[ofset + 17], textContent[ofset + 18]);
			ssd1306_putstring(10, 46, disText);
		break;
		case 2: // soil
			ssd1306_drawbitmap(arrow_single);
		  ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", targetval[1]);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", activeper[1]);
			ssd1306_putstring(110, 26, temp);
		break;
		case 3: // illu
			ssd1306_drawbitmap(arrow_single);
			ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", targetval[2]);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", activeper[2]);
			ssd1306_putstring(110, 26, temp);
		break;
		case 4: // pre
			menu_Level = 3;
		break;
		default:
			ssd1306_drawbitmap(arrow_single);
			ssd1306_putstring(10, 6, "CHANGE TARGET");
			
			ssd1306_putstring(10, 16, "CURRENT TARGET:");
			sprintf(temp, "%d", targetval[0]);
			ssd1306_putstring(110, 16, temp);
		
			ssd1306_putstring(10, 26, "CURRENT:");
			sprintf(temp, "%d", activeper[0]);
			ssd1306_putstring(110, 26, temp);
		break;
	}
	
	ssd1306_update();
	
}

void menu_Level2() {
	ssd1306_clearscreen();
	
	// checking the buttons
	if (sw1_pressed_flag == true) {
		scrollBack_timer = 0;
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
		scrollBack_timer = 0;
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
			targetval[0] = changeValue;
		break;
		case 1:
			targetval[3] = changeValue;
		break;
		case 2:
			targetval[1] = changeValue;
		break;
		case 3:
			targetval[2] = changeValue;
		break;
	}
	
	//check if the user is ready
	if (sw3_pressed_flag == true) {
		menu_Level = 0;
		sw3_pressed_flag = false;
		wentBack1 = true;	
		node_sendstring(0);
	}
	
	//chect if the user wants to go back
	if (sw2_pressed_flag == true) {
		menu_Level = 1;
		sw2_pressed_flag = false;
		node_sendstring(0);
	}
	
	
	ssd1306_update();
	
	
}

void menu_Preset() {
	
	//checking if the user wants to go back
	if (sw2_pressed_flag == true) {
		menu_Level = 0;
		sw2_pressed_flag = false;
		wentBack1 = true;
	}
	
	// handeling the user input
	if (sw1_pressed_flag == true) {
		scrollBack_timer = 0;
		if (vert_cursor3 > 0) {
			vert_cursor3--;
			sw1_pressed_flag = false;
		} else {
			sw1_pressed_flag = false;
		}
	}
	if (sw4_pressed_flag == true) {
		scrollBack_timer = 0;
		if (vert_cursor3 < 2) {
			vert_cursor3++;
			sw4_pressed_flag = false;
		} else {
			sw4_pressed_flag = false;
		}
	}
	
	sw2_pressed_flag = false;
	sw5_pressed_flag = false;
	
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
			default:
				ssd1306_drawbitmap(arrow_select1);
			break;
		}
		ssd1306_putstring(10, 6,  "PRESET1");
		ssd1306_putstring(10, 16, "PRESET2");
		ssd1306_putstring(10, 26, "PRESET3");

		
		ssd1306_update();
		
		//if the user has updated its preferance act acordingly
		if (sw3_pressed_flag == true) {
		menu_Level = 0;
		sw3_pressed_flag = false;
		wentBack1 = true;	
			switch(vert_cursor3) {
			case 0:
				targetval[0] = 65;
				targetval[1] = 35;
				targetval[2] = 20;
				targetval[3] = 30;
			break;
			case 1:
				targetval[0] = 70;
				targetval[1] = 62;
				targetval[2] = 5;
				targetval[3] = 23;
			break;
			case 2:
				targetval[0] = 85;
				targetval[1] = 50;
				targetval[2] = 15;
				targetval[3] = 25;
			break;
			default:
				targetval[0] = 60;
				targetval[1] = 56;
				targetval[2] = 18;
				targetval[3] = 24;
			break;
		}
		node_sendstring(0);
	}
		
}
