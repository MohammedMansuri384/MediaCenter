#include <LPC17xx.H>
#include "GLCD.h"
#include "KBD.h"

static void delay (){
	int i=0;
	int j=0;
	for (i=0; i<800; i++){
		for (j=0; j<800; j++){}
	}
	}

typedef struct{
	int x,y;
	int old_x,old_y;
	int w,h;
	unsigned short color;
}Paddle;

typedef struct{
	int x,y;
	int old_x,old_y;
	int size;
	int vx,vy;
	unsigned short color;
}Ball;

typedef struct{
	int x,y;
	int w,h;
	int is_hit;
	unsigned short color;
}Brick;

enum GameState{
		STATE_PLAYING,
		STATE_WIN,
		STATE_LOSE
};

#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240
#define PADDLE_WIDTH   60
#define PADDLE_HEIGHT  10
#define BALL_SIZE      5
#define BRICK_WIDTH    30
#define BRICK_HEIGHT   15
#define NUM_BRICK_ROWS 4
#define NUM_BRICK_COLS 10

Paddle paddle;
Ball ball;
Brick bricks[NUM_BRICK_ROWS][NUM_BRICK_COLS];
int bricks_remaining;
enum GameState current_state;

int r=0;  
int c=0;

void game_init(void){
	paddle.w = PADDLE_WIDTH;
	paddle.h = PADDLE_HEIGHT;
	paddle.x = (SCREEN_WIDTH / 2)-(paddle.w / 2);
	paddle.y = (SCREEN_HEIGHT - paddle.h - 5);
	paddle.old_x = paddle.x;
	paddle.old_y = paddle.y;
	paddle.color = Blue;
	
	ball.size = BALL_SIZE;
    ball.x = (SCREEN_WIDTH / 2) - (ball.size / 2);
    ball.y = paddle.y - ball.size - 1;            
    ball.old_x = ball.x;
    ball.old_y = ball.y;
    ball.vx = 2; 
    ball.vy = -2;
    ball.color = Red;
    
    bricks_remaining = NUM_BRICK_ROWS * NUM_BRICK_COLS;
  
	for (r = 0; r < NUM_BRICK_ROWS; r++) {
        for (c = 0; c < NUM_BRICK_COLS; c++) {
            bricks[r][c].w = BRICK_WIDTH;
            bricks[r][c].h = BRICK_HEIGHT;
            bricks[r][c].x = (c * (BRICK_WIDTH + 2)) + 5;
            bricks[r][c].y = (r * (BRICK_HEIGHT + 2)) + 10;
            bricks[r][c].is_hit = 0;
            bricks[r][c].color = (r % 2 == 0) ? Green : Yellow;
        }
    }
    current_state = STATE_PLAYING;
}

void game_draw(void){
	GLCD_Clear(Black);
	GLCD_FillRect(paddle.x,paddle.y,paddle.w,paddle.h,paddle.color);
	GLCD_FillRect(ball.x,ball.y,ball.size,ball.size,ball.color);
	
	for (r = 0; r < NUM_BRICK_ROWS; r++) {
        for (c = 0; c < NUM_BRICK_COLS; c++) {
            if (!(bricks[r][c].is_hit)) {
                GLCD_FillRect(bricks[r][c].x, bricks[r][c].y, 
                              bricks[r][c].w, bricks[r][c].h, 
                              bricks[r][c].color);
            }
        }
	}
}

int check_collision(int x1,int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	if (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2){
		return 1;
	}
	return 0;
}

int brick_breaker(void){
	int joystick_val = 0;
	game_init();
	game_draw();
	
	SysTick_Config(SystemCoreClock/100);
	
	while(1){
		switch(current_state){
			case STATE_PLAYING:
				joystick_val = get_button();
				paddle.old_x=paddle.x;
				
				if(joystick_val == KBD_LEFT){
					paddle.x-=4;
				}
				if(joystick_val == KBD_RIGHT){
					paddle.x+=4;
				}
				
				if (paddle.x < 0) paddle.x = 0;
				if (paddle.x + paddle.w > SCREEN_WIDTH){
				paddle.x= SCREEN_WIDTH - paddle.w;
				}
				
				ball.old_x = ball.x;
				ball.old_y = ball.y;
				ball.x += ball.vx;
				ball.y += ball.vy;
				
				if (ball.x <= 0 || ball.x + ball.size >= SCREEN_WIDTH){
					ball.vx = -ball.vx;
				}
				if (ball.y <= 0){
					ball.vy = -ball.vy;
				}
				
				if (ball.y + ball.size >= SCREEN_HEIGHT){
					current_state = STATE_LOSE;
					break;
				}
				
				if (check_collision(ball.x,ball.y,ball.size,ball.size,paddle.x,paddle.y,paddle.w,paddle.h)){
					ball.vy=-ball.vy;
				}
				
				for (r = 0; r < NUM_BRICK_ROWS; r++) {
                    for (c = 0; c < NUM_BRICK_COLS; c++) {
                        if (!(bricks[r][c].is_hit) && 
                            check_collision(ball.x, ball.y, ball.size, ball.size,
                                            bricks[r][c].x, bricks[r][c].y, 
                                            bricks[r][c].w, bricks[r][c].h)) {
                            
                            ball.vy = -ball.vy;
                            bricks[r][c].is_hit = 1;
                            bricks_remaining--;

                            GLCD_FillRect(bricks[r][c].x, bricks[r][c].y, 
                                          bricks[r][c].w, bricks[r][c].h, 
                                          Black);
                            goto check_win;
                        }
                    }
                }
                
                check_win:
					if (bricks_remaining==0){
						current_state=STATE_WIN;
						break;
					}
					GLCD_FillRect(paddle.old_x, paddle.y, paddle.w, paddle.h, Black);

					GLCD_FillRect(paddle.x, paddle.y, paddle.w, paddle.h, paddle.color);
                
					GLCD_FillRect(ball.old_x, ball.old_y, ball.size, ball.size, Black);

					GLCD_FillRect(ball.x, ball.y, ball.size, ball.size, ball.color);

					
					break;
			case STATE_WIN:
					GLCD_Clear(Green);
					GLCD_SetBackColor(Green);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(4,8,1, "YOU WIN");
					GLCD_DisplayString(6,1,1, "Play Again (UP)");
					GLCD_DisplayString(7,1,1, "Main Menu (Left)");
					while(1){
						joystick_val = get_button();
						if (joystick_val == KBD_UP){
						game_init();
						game_draw();
						break;
						}
						if (joystick_val == KBD_LEFT){
							return 0;
						}
					}
					break;
			case STATE_LOSE:
					GLCD_Clear(Red);
					GLCD_SetBackColor(Red);
					GLCD_SetTextColor(White);
					GLCD_DisplayString(4,7,1, "GAME OVER");
					GLCD_DisplayString(6,1,1, "Play Again (UP)");
					GLCD_DisplayString(7,1,1, "Game Menu (Left)");
					while(1){
						joystick_val = get_button();
						if (joystick_val == KBD_UP){
						game_init();
						game_draw();
						break;
						}
						if (joystick_val == KBD_LEFT){
							GLCD_Clear(White);
							return (0);
						}
					}
					break;
		}
		delay();
		
	}
}
