#include <LPC17xx.H>
#include "GLCD.h"
#include <stdlib.h>
#include <stdio.h>
#include "KBD.h"

extern int check_collision (int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

typedef struct{
	int x,y;
	int old_y;
	int w,h;
	int vy;
	unsigned short color;
}Player;

typedef struct {
    int x, old_x;
    int w;
    int gap_y;
    int gap_h;
    int passed;
    unsigned short color;
}Pipe;

enum GameState {
    STATE_START,
    STATE_PLAYING,
    STATE_LOSE
};

int i=0;

#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240

#define PLAYER_START_X 80
#define PLAYER_START_Y 120
#define PLAYER_W 15
#define PLAYER_H 15
#define GRAVITY 1 
#define FLAP_STRENGTH -10 

#define NUM_PIPES 3 
#define PIPE_W 30 
#define PIPE_SPEED 2   
#define PIPE_GAP_MIN 70 
#define PIPE_GAP_MAX 100 
#define PIPE_SPACING 130

Player player;
Pipe pipes[NUM_PIPES];
int score;
enum GameState current_state_bird;
char score_str[20];

void randomize_gap(int i){
	pipes[i].gap_h = (rand() % (PIPE_GAP_MAX - PIPE_GAP_MIN)) + PIPE_GAP_MIN;
	
	pipes[i].gap_y = (rand() % (SCREEN_HEIGHT - pipes[i].gap_h - 40)) + 20;
	
	pipes[i].passed=0;

}

void game_init_bird(void){
	player.w=PLAYER_W;
	player.h=PLAYER_H;
	player.x=PLAYER_START_X;
	player.y=PLAYER_START_Y;
	player.old_y=player.y;
	player.vy=0;
	player.color=Yellow;
	
	for (i = 0; i < NUM_PIPES; i++) {
        pipes[i].w = PIPE_W;
        pipes[i].x = SCREEN_WIDTH + (i * PIPE_SPACING);
        pipes[i].old_x = pipes[i].x;
        pipes[i].color = Green;
        randomize_gap(i);
	}
	score=0;
	current_state_bird=STATE_START;

}

void game_start(void){
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(4,4,1, "Flappy Rectangle");
	GLCD_DisplayString(7,3,1, "Press Up to Begin");
	GLCD_FillRect(player.x,player.y,player.w,player.h,player.color);
}

void draw_pipe(int i, unsigned short color){
	int bot_rect_y = pipes[i].gap_y + pipes[i].gap_h;
    int bot_rect_h = SCREEN_HEIGHT - bot_rect_y;
	GLCD_FillRect(pipes[i].x, 0, pipes[i].w, pipes[i].gap_y, color);
    GLCD_FillRect(pipes[i].x, bot_rect_y, pipes[i].w, bot_rect_h, color);
}

int flappy_bird(void){
	int joystick_val=0;
	int joystick_prev=0;
	game_init_bird();
	game_start();
	
	
	
	while(1){
		joystick_val=get_button();
		joystick_prev=joystick_val;
		switch(current_state_bird){
			case STATE_START:
				if(joystick_val==KBD_UP){
					player.vy = FLAP_STRENGTH;
					current_state_bird = STATE_PLAYING;
					GLCD_Clear(White);
					
					sprintf(score_str, "Score %d",score);
					GLCD_DisplayString(0,0,1,(unsigned char*)score_str);
				}
				break;
			case STATE_PLAYING:
					if (joystick_val==KBD_UP){
						player.vy = FLAP_STRENGTH;
					}
					
					player.old_y=player.y;
					player.vy += GRAVITY;
					player.y+=player.vy;
					
					for (i = 0; i < NUM_PIPES; i++) {
						pipes[i].old_x = pipes[i].x;
						pipes[i].x -= PIPE_SPEED;
                    
						if (pipes[i].x + pipes[i].w < 0) {
							int last_pipe_x = pipes[(i + NUM_PIPES - 1) % NUM_PIPES].x;
							pipes[i].x = last_pipe_x + PIPE_SPACING;
							randomize_gap(i);
						}
					}
					
					if (player.y<0 || (player.y + player.h)>SCREEN_HEIGHT){
						current_state_bird=STATE_LOSE;
						break;
					}
					
					for (i = 0; i < NUM_PIPES; i++) {
  
						int top_rect_h = pipes[i].gap_y;
						int bot_rect_y = pipes[i].gap_y + pipes[i].gap_h;
						int bot_rect_h = SCREEN_HEIGHT - bot_rect_y;

						if (check_collision(player.x, player.y, player.w, player.h, pipes[i].x, 0, pipes[i].w, top_rect_h)) {
							current_state_bird = STATE_LOSE;
							break;
						}
						if (check_collision(player.x, player.y, player.w, player.h, pipes[i].x, bot_rect_y, pipes[i].w, bot_rect_h)) {
							current_state_bird = STATE_LOSE;
							break;
						}
					}
                if (current_state_bird == STATE_LOSE) break;
                
                for (i = 0; i < NUM_PIPES; i++){
					if(!pipes[i].passed && player.x > (pipes[i].x + pipes[i].w)){
						pipes[i].passed=1;
						score++;
						
						sprintf(score_str, "Score: %d",score);
						GLCD_FillRect(0,0,150,10,White);
						GLCD_DisplayString(0,0,1,(unsigned char*)score_str);
					}
				}
				
				GLCD_FillRect(player.x,player.old_y,player.w,player.h,White);
				GLCD_FillRect(player.x,player.y,player.w,player.h,player.color);
                
                for (i = 0; i < NUM_PIPES; i++) {
                  int bot_y = pipes[i].gap_y + pipes[i].gap_h;  
									GLCD_FillRect(pipes[i].old_x, 0, pipes[i].w, pipes[i].gap_y, White);
                    
                    GLCD_FillRect(pipes[i].old_x, bot_y, pipes[i].w, SCREEN_HEIGHT - bot_y, White);
                    
                    draw_pipe(i, pipes[i].color);
										sprintf(score_str, "Score: %d",score);
										GLCD_DisplayString(0,0,1,(unsigned char*)score_str);
                }
                
                break;
			case STATE_LOSE:
				GLCD_Clear(Red);
				GLCD_SetBackColor(Red);
				GLCD_SetTextColor(White);
				
				GLCD_DisplayString(4,7,1, "GAME OVER");
				
				sprintf(score_str, "Score: %d",score);
				GLCD_DisplayString(5,6,1, (unsigned char*)score_str);
				GLCD_DisplayString(7,1,1, "Play Again (UP)");
				GLCD_DisplayString(8,1,1, "Game Menu (LEFT)");
				while(1){
					joystick_val=get_button();
					if (joystick_val==KBD_UP){
						game_init_bird();
						game_start();
						break;
					}
					if (joystick_val==KBD_LEFT){
						GLCD_Clear(White);
						return (0);
					}
				}
				break;
		}
		//delay
	}

}
