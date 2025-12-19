#include <stdio.h>
#include "LPC17xx.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

int f;
static void game_page(int selector) {
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Black);
    GLCD_DisplayString (0, 0, 1, "      Game Menu     ");
    GLCD_DisplayString (1, 0, 1, "   Select A Game     ");
    GLCD_DisplayString (8, 0, 1, "Use Joystick Up/Down");
    GLCD_DisplayString (9, 0, 1, " Right:  To Choose ");

    GLCD_SetBackColor(White);
    GLCD_DisplayString(4,0,1, "    Brick Breaker");
    GLCD_DisplayString(5,0,1, " Flappy Rectangle");
    GLCD_DisplayString(6,0,1, "        Main Menu");

    GLCD_SetBackColor(Blue);
    if (selector == 1) {
                GLCD_DisplayString(4,0,1, "    Brick Breaker");
        } else if (selector == 2) {
                GLCD_DisplayString(5,0,1, " Flappy Rectangle");
        } else if (selector == 3) {
                GLCD_DisplayString(6,0,1, "        Main Menu");
        }
    GLCD_SetBackColor(White);
}

extern int brick_breaker(void);
extern int flappy_bird(void);

int game_menu(void) {
    
    int selector = 1;
    int joystick_val;

    SystemInit();
    SystemCoreClockUpdate();
    GLCD_Init();
    GLCD_Clear(White);
    KBD_Init();
    LED_Init();

    game_page(selector);

    while (1) {
        joystick_val = get_button();

        if (joystick_val != 0) {
            if (joystick_val == KBD_DOWN) {
                            
                if (selector < 3) selector++;
                game_page(selector);
							  for (f=0;f<=3;f++){
								LED_Off(f);
								}
							  LED_On(selector);
                            
            } else if (joystick_val == KBD_UP) {
                            
                if (selector > 1) selector--;
                game_page(selector);
							  for (f=0;f<=3;f++){
								LED_Off(f);
								}
							  LED_On(selector);
                            
            } else if (joystick_val == KBD_RIGHT) {
                            
                                if (selector == 1) {
                                    GLCD_Clear(White);
                                    brick_breaker();
                                    
                                    game_page(selector);
                                } else if (selector == 2) {
																	GLCD_Clear(White);
																	flappy_bird();
                                } else if (selector == 3) {
																	GLCD_Clear(White);
																	return 0;
                                }
            }
        }
    }
}