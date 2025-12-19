#include <LPC17xx.H>
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#include "forest.c"
#include "reef.c" 
#include "hill.c"
#include "lake.c"

#define BORDER_SIZE 3

unsigned int img_x[5] = { 0, 10,  10, 170,  170 };
unsigned int img_y[5] = { 0, 10,  114,  10, 114 };

int selector();
int selected();

extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned int counter;

extern int images(int x);

void draw_border(int index, unsigned short color) {
  unsigned int x = img_x[index];
    unsigned int y = img_y[index];  
	if (index < 1 || index > 4) return;


    GLCD_FillRect(x - BORDER_SIZE, y - BORDER_SIZE, IMG_W + 2*BORDER_SIZE, BORDER_SIZE, color);
    
    GLCD_FillRect(x - BORDER_SIZE, y + IMG_H, IMG_W + 2*BORDER_SIZE, BORDER_SIZE, color);

    GLCD_FillRect(x - BORDER_SIZE, y, BORDER_SIZE, IMG_H, color);

    GLCD_FillRect(x + IMG_W, y, BORDER_SIZE, IMG_H, color);
}

void photo_menu(){
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0,4,1, "Photo Gallery");
		GLCD_Bitmap(10,10,,HILL_WIDTH, HILL_HEIGHT,HILL_PIXEL_DATA);
		GLCD_Bitmap(10,114,REEF_WIDTH,REEF_HEIGHT,REEF_PIXEL_DATA);
		GLCD_Bitmap(170,10,LAKE_WIDTH,LAKE_HEIGHT,LAKE_PIXEL_DATA);
		GLCD_Bitmap(170,114,FOREST_WIDTH,FOREST_HEIGHT,FOREST_PIXEL_DATA);
		GLCD_DisplayString(8,0,1,"Right to Select");
		GLCD_DisplayString(9,0,1,"Left to Main Menu");
}

int photo_gallery(void){
	int selector=1;
	int selected=0;
	
	int joystick_val=0;
	int joystick_prev=0;
	
	KBD_Init();
	LED_Init();
	GLCD_Clear(White);
	photo_menu();
	

	while(1){
		joystick_val = get_button();

	if (joystick_val!=joystick_prev){
		if (joystick_val == KBD_DOWN){
			if (selector>4){
				selector=1;			
			}
			else {
				selector+=1;
			}		
		}
		else if (joystick_val == KBD_UP){
			if (selector<1){
				selector=4;
			}
			else{
				selector-=1;
			}
		}
		
		if (selector!=selected){
				draw_border(selected, White);
				draw_border(selector, Blue);
				selected=selector;
		}
		if (joystick_val == KBD_RIGHT){
			switch(selector){
			case 1:
				GLCD_Clear(White);
				images(selector);
			case 2:
				GLCD_Clear(White);
				images(selector);
			case 3:
				GLCD_Clear(White);
				images(selector);
			case 4:
				GLCD_Clear(White);
				images(selector);
			}
			photo_menu();
			draw_border(selector,Blue);
		}
		else if (joystick_val == KBD_LEFT){
			GLCD_Clear(White);
			return(0);
		}
		joystick_prev = joystick_val;	
	}
	}

}
