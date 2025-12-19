#include <stdio.h>
#include "LPC17xx.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

static void main_menu(int selector) {
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Black);
    GLCD_DisplayString (0, 0, 1, "COE718 Final Project");
    GLCD_DisplayString (1, 0, 1, "      Main Menu     ");
    GLCD_DisplayString (8, 0, 1, "Use Joystick Up/Down");
    GLCD_DisplayString (9, 0, 1, " Right:  To Choose ");

    GLCD_SetBackColor(White);
    GLCD_DisplayString(4,0,1, "    Photo Gallery");
    GLCD_DisplayString(5,0,1, "    Audio Player");
    GLCD_DisplayString(6,0,1, "    Game Menu");

    GLCD_SetBackColor(Blue);
    if (selector == 1) {
                GLCD_DisplayString(4,0,1, "    Photo Gallery");
        } else if (selector == 2) {
                GLCD_DisplayString(5,0,1, "    Audio Player");
        } else if (selector == 3) {
                GLCD_DisplayString(6,0,1, "    Game Menu");
        }
    GLCD_SetBackColor(White);
}

extern int photo_gallery(void);
extern int game_menu(void);
extern int audio_main(void);

int main(void) {
    
    int selector = 1;
    int joystick_val;
		int i;
    SystemInit();
    SystemCoreClockUpdate();
    GLCD_Init();
    GLCD_Clear(White);
    KBD_Init();
    LED_Init();

    main_menu(selector);

    while (1) {
        joystick_val = get_button();

        if (joystick_val != 0) {
            if (joystick_val == KBD_DOWN) {
                            
                if (selector < 3) selector++;
                main_menu(selector);
							  for (i=0;i<=3;i++){
								LED_Off(i);
								}
							  LED_On(selector);
                            
            } else if (joystick_val == KBD_UP) {
                            
                if (selector > 1) selector--;
                main_menu(selector);
							for (i=0;i<=3;i++){
								LED_Off(i);
								}
                LED_On(selector);
            } else if (joystick_val == KBD_RIGHT) {
                            
                                if (selector == 1) {
                                    GLCD_Clear(White);
                                    photo_gallery();
                                    
                                    main_menu(selector);
                                } else if (selector == 2) {
																	GLCD_Clear(White);
																	audio_main();
                                } else if (selector == 3) {
																	GLCD_Clear(White);
																	game_menu();
                                }
            }
        }
    }
}