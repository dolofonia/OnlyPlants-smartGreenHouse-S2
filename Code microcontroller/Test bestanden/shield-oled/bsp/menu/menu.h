#ifndef MENU_H
#define MENU_H

extern volatile int humidity_Current;
extern volatile int humidity_Target;

extern volatile int soil_Moisture_Current;
extern volatile int soil_Moisture_Target;

extern volatile int illumination_Current;
extern volatile int illumination_Target;

extern volatile int temperature_Current;
extern volatile int temperature_Target;

void menu_init();
void menu_Update();

#endif