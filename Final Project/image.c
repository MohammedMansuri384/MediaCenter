#include <LPC17xx.H>
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#include "forest.c"
#include "reef.c"
#include "lake.c"
#include "hill.c"

extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned int counter;

int images(int x){
		int joystick_val=0;
		int joystick_prev=0;
		
		KBD_Init();
		LED_Init();
		
		GLCD_Clear(White);
		
		while(1){
			joystick_val=get_button();
			switch (x){
				case 1:
					GLCD_DisplayString(0,0,1, "Hill");
					GLCD_Bitmap(10,15,HILL_WIDTH,HILL_HEIGHT,HILL_PIXEL_DATA);
					LED_On(x);
				case 2:
					GLCD_DisplayString(0,0,1, "Reef");
					GLCD_Bitmap(10,15,REEF_WIDTH,REEF_HEIGHT,REEF_PIXEL_DATA);
					LED_On(x);
				case 3:
					GLCD_DisplayString(0,0,1, "Lake");
					GLCD_Bitmap(26,15,LAKE_WIDTH,LAKE_HEIGHT,LAKE_PIXEL_DATA);
					LED_On(x);
				case 4:
					GLCD_DisplayString(0,0,1, "Forest");
					GLCD_Bitmap(10,15,FOREST_WIDTH,FOREST_HEIGHT,FOREST_PIXEL_DATA);
					LED_On(x);
			}
			if (joystick_val!=joystick_prev){
				if (joystick_val == KBD_LEFT){
					LED_Off(x);
					GLCD_Clear(White);
					return(0);
				}
			}
		
		}
}
