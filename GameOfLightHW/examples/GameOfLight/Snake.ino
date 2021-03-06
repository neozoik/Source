/*
  Snake, a game for the GameOfLight libraries
  Copyright (c) 2013 Stian Selbek.  All right reserved.

  This file is part of Game Of Light.

  Game Of Light is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Game Of Light is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Game Of Light.  If not, see <http://www.gnu.org/licenses/>.
*/

//TODO:
//Menu on start
//Change press start behaviour to account for player1 not necessarily joining
//Best of 3, best of 5.
//Win conditions match:
  //Player reaches 32 points
  //Player gains the amount of food eaten in a match as points + bonus for last person standing
//Win conditions:
  //Player is over 99 points total
  //Player has the most points after X games

#include <GameOfLightHW.h>

//From the main menu:
extern GameOfLightHW frame;
extern int8_t player[4]; //must call menu_playerStart for valid content
extern uint8_t playerCnt; //must call menu_playerStart for valid count
extern void menu_playerStart(uint8_t minPlayers, int8_t press_start_line, const int8_t player_x[4], const int8_t player_line[4]);

//Position check for being outside the score areas
#define IS_ILLEGAL_FOOD_POS(x, y) ((y < 8 || y > 55) && (x < 13 || x > 50))

#define MAXSEGMENTS 63 //maximum number of segments in a snake
#define SPEEUP_FACTOR 4 //The nr of ms framedelay to remove per food eaten
//#define PLAYER1 0
//#define PLAYER2 1

uint8_t snake_splashscrn[] PROGMEM = {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xc0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0,
0x48, 0x0, 0x3c, 0x0, 0x1c, 0x0, 0xe, 0x0, 0x6, 0x0, 0xe, 0x0, 0xee, 0x0, 0xff, 0x0,
0xff, 0x0, 0xff, 0x0, 0x7e, 0x0, 0xfc, 0x0, 0xf8, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x38, 0x0, 0xfe, 0x0, 0xf8, 0x0, 0xf8, 0x0, 0xfc, 0x0, 0xfe, 0x0, 0xff, 0x0,
0xf8, 0x0, 0xf0, 0x0, 0xf3, 0x3, 0xf9, 0x1, 0x7c, 0x0, 0x1f, 0x0, 0xe6, 0x0, 0xf8, 0x0,
0xfe, 0x0, 0xfe, 0x0, 0x7f, 0x0, 0x3f, 0x80, 0x1f, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x10, 0x0, 0x50, 0x0, 0x50, 0x1, 0x30, 0x1, 0x18, 0x1, 0x8, 0x1, 0x4, 0x1, 0x0,
0x81, 0x80, 0xe1, 0xe0, 0xe8, 0xe8, 0x2e, 0x2e, 0xce, 0xe, 0xfc, 0x0, 0xff, 0x0, 0x7f, 0x0,
0x7f, 0x80, 0x3e, 0x80, 0x1c, 0x1, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x80, 0x0, 0x0, 0x40, 0x0, 0xe0, 0x0, 0xe0, 0x0, 0x0,
0xf0, 0x0, 0xe0, 0x0, 0xe0, 0x0, 0x80, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xc0, 0xe0, 0xe0, 0xee, 0xee,
0xee, 0xee, 0xe, 0xe, 0xf8, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0x7f, 0x0, 0x3e, 0x0,
0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0xc0, 0x10, 0x80, 0x1c,
0x80, 0x3c, 0x80, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xfe, 0x0, 0xfc, 0x0, 0xfe, 0x0,
0x3f, 0x0, 0xff, 0x0, 0xff, 0x0, 0xfb, 0x0, 0x78, 0x3, 0x18, 0x1, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x38, 0x38, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd,
0xbd, 0xbd, 0x0, 0x0, 0x7f, 0x0, 0xff, 0x0, 0xff, 0x0, 0xfe, 0x0, 0xfc, 0x1, 0xe0, 0x0,
0xe0, 0x0, 0xf0, 0x4, 0xf0, 0x4, 0xf0, 0xf, 0xe0, 0xe, 0xf0, 0x0, 0xff, 0x0, 0xff, 0x0,
0xff, 0x0, 0x7f, 0x0, 0x3f, 0x0, 0x1f, 0x0, 0x7, 0x0, 0x3, 0x0, 0x7d, 0x7c, 0x7e, 0x7e,
0x1f, 0x0, 0x7f, 0x0, 0xff, 0x0, 0xff, 0x0, 0xfc, 0x0, 0xc0, 0x1, 0xf0, 0x4, 0xe0, 0xc,
0xfe, 0x0, 0x7e, 0x1, 0x3e, 0x0, 0xf, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x3, 0xf, 0xf, 0x17, 0x17,
0x17, 0x17, 0x37, 0x37, 0x3e, 0x3e, 0x19, 0x18, 0x1b, 0x18, 0x33, 0x30, 0x27, 0x20, 0x27, 0x20,
0x27, 0x20, 0x27, 0x20, 0x17, 0x10, 0x7, 0x0, 0x3, 0x0, 0x3, 0x0, 0x3, 0x0, 0x1, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x3,
0x3, 0x3, 0x2, 0x2, 0x8, 0x8, 0xd, 0xc, 0x1, 0x0, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

//Segments are stored as directions only. snake_seg[0] is the tail of the snake. 4 directions per byte.
uint8_t snake_seg[4][(MAXSEGMENTS+1) / 4]; //Direction of segments
uint8_t snake_tailPosX[4];
uint8_t snake_tailPosY[4];

uint8_t snake_head[4]; //position of head in the buffer
#define snake_tail MAXSEGMENTS

uint8_t snake_count[4]; //nr of segments
uint8_t snake_headPosX[4]; //position of head on the board
uint8_t snake_headPosY[4];
	
uint8_t foodX;
uint8_t foodY;
uint8_t food_timer;

uint8_t delayT; // time between each frame of the game
uint8_t rand_8();

//Xorshift psuedorandomizer variables. Values from http://en.wikipedia.org/wiki/Xorshift truncated to 8-bits.
uint8_t randX = 21;
uint8_t randY = 229;
uint8_t randZ = 181;
uint8_t randW = 51;


uint8_t rand_8() {
	uint8_t tmp = randX ^ (randX << 3);
	randX = randY;
	randY = randZ;
	randZ = randW;
	randW ^= (randW >> 2) ^ (tmp ^ (tmp >> 3));

	return randW;
}


int8_t snake_dirToX(uint8_t dir) {
	return dir == WEST ? -1 : (dir == EAST ? 1 : 0);
}

int8_t snake_dirToY(uint8_t dir) {
	return dir == NORTH ? -1 : (dir == SOUTH ? 1 : 0);
}


void snake_drawBorder() {
	//Draw sides:
	frame.drawLine(13, 0, 50, 0, ORANGE);
	frame.drawLine(13, 63, 50, 63, ORANGE);
	frame.drawLine(0, 8, 0, 55, ORANGE);
	frame.drawLine(63, 8, 63, 55, ORANGE);

	//Cornerboxes along top:
	frame.drawLine(13, 0, 13, 8, ORANGE);
	frame.drawLine(50, 0, 50, 8, ORANGE);
	frame.drawLine(0, 8, 13, 8, ORANGE);
	frame.drawLine(50, 8, 63, 8, ORANGE);

	//Cornerboxes along bottom:
	frame.drawLine(13, 55, 13, 63, ORANGE);
	frame.drawLine(50, 55, 50, 63, ORANGE);
	frame.drawLine(0, 55, 13, 55, ORANGE);
	frame.drawLine(50, 55, 63, 55, ORANGE);

	//Draw holes in the border to allow the snakes to wrap
	frame.drawLine(0, 31, 0, 32, BLACK);
	frame.drawLine(63, 31, 63, 32, BLACK);
	frame.drawLine(31, 0, 32, 0, BLACK);
	frame.drawLine(31, 63, 32, 63, BLACK);
}



void snake_addSeg(const uint8_t player) {
	if (snake_head[player] == MAXSEGMENTS - 1) return; //Buffer full!
	//snake_head[player]--;
	
	/*snakeX[player][snake_head[player]] = snake_headPosX[player];
	snakeY[player][snake_head[player]] = snake_headPosY[player];
	frame.setPixel(snake_headPosX[player], snake_headPosY[player], player+1);*/
	snake_count[player]++;
}


uint8_t snake_checkCollision(uint8_t playerCurr, uint8_t p_alive[4]) {
	uint8_t i;
	if (frame.getPixel(snake_headPosX[playerCurr], snake_headPosY[playerCurr])) {
		//Checks new head location for collision
		return 1;
	}
	//Check all snake head locations as they haven't been painted yet, but will lead to a collision
	for (i = 0; i < playerCnt; i++) {
		int8_t other = player[i];
		if (other != playerCurr && p_alive[other] && (snake_headPosX[playerCurr] == snake_headPosX[other]) && (snake_headPosY[playerCurr] == snake_headPosY[other])) {
			return 1;
		}
	}
	return 0;
}


void snake_newFood() {
	food_timer = 0;
	do {
		foodX = rand_8()>>2; //max 64
		foodY = rand_8()>>2; //max 64
	} while (frame.getPixel(foodX, foodY) || IS_ILLEGAL_FOOD_POS(foodX, foodY));
	delayT -= SPEEUP_FACTOR; //speed up game
	frame.setPixel(foodX, foodY, ORANGE);
}


void snake_setSegDir(uint8_t player, uint8_t segment, uint8_t dir) {
	//Packs the used direction in the byte array
	snake_seg[player][segment/4] = (snake_seg[player][segment/4] & ~(0x03 << ((segment % 4)*2))) | (dir << ((segment % 4)*2));
}


uint8_t snake_getSegDir(uint8_t player, uint8_t segment) {
	//Unpacks the used direction from the packed byte array
	return (snake_seg[player][segment/4] >> ((segment % 4)*2)) & 0x03;
}


uint8_t snake_getLastDir(uint8_t player) {
	//Returns the last used direction of this player
	return snake_getSegDir(player, snake_head[player]);
}


uint8_t snake_moveHandler(uint8_t player) {
	uint8_t dir;
	dir = frame.getDir(player);
	
	if (dir == 4 || (dir == (snake_getLastDir(player) ^ 3))) {
		//Checking lastdir != dir XOR 3 to avoid colliding with self
		dir = snake_getLastDir(player);
	}
	snake_headPosX[player] += snake_dirToX(dir);
	snake_headPosY[player] += snake_dirToY(dir);

	//To allow the snake to use the shortcuts along the edges
	snake_headPosX[player] &= 0x3F;
	snake_headPosY[player] &= 0x3F;

	return dir;
}


void snake_move(uint8_t player) {
	uint8_t dir = snake_moveHandler(player);

	//Blank the pixel containing the tailsegment
	frame.setPixel(snake_tailPosX[player], snake_tailPosY[player], 0);
	
	//if (snake_tail - snake_head[player] + 1 != snake_count[player]) {
	if (snake_head[player] != snake_count[player] - 1) {
		//Has eaten, body remains in place
		snake_head[player]++;
	} else {
		//Move entire snake one step forward

		for (uint8_t i = 0; i <= (snake_count[player] / 4); i++) {
			snake_seg[player][i] = (snake_seg[player][i] >> 2) | (snake_seg[player][i+1] << 6);
		}
		snake_tailPosX[player] += snake_dirToX(snake_seg[player][0] & 0x03);
		snake_tailPosY[player] += snake_dirToY(snake_seg[player][0] & 0x03);

		snake_tailPosX[player] &= 0x3F;
		snake_tailPosY[player] &= 0x3F;
	}
	//save used direction
	snake_setSegDir(player, snake_head[player], dir);
	//Don't draw the head yet, to aid in collisionchecking!
}


void snake_reDraw(uint8_t player) {
	//Draws the initial snake. Only for use with the initial configuration
	frame.drawLine(snake_headPosX[player], snake_headPosY[player], snake_tailPosX[player], snake_tailPosY[player], (player < 2) ? GREEN : RED);
	frame.setPixel(snake_headPosX[player], snake_headPosY[player],  (player & 1) ? ORANGE : (player < 2 ? GREEN : RED));
}


uint8_t isStartPressed() {
	if (frame.getStart(PLAYER1)) {
		return 1;
	}
	return 0;
}


void snake_startPos(uint8_t player, uint8_t xPos, uint8_t yPos, uint8_t dir) {
	int8_t dirX = snake_dirToX(dir);
	int8_t dirY = snake_dirToY(dir);

	//Initializes the body of a snake to the given position.
	snake_headPosX[player] = xPos;
	snake_headPosY[player] = yPos;
	snake_setSegDir(player, 0, dir);
	snake_setSegDir(player, 1, dir);
	snake_setSegDir(player, 2, dir);
	snake_tailPosX[player] = xPos - 2*dirX;
	snake_tailPosY[player] = yPos - 2*dirY;
	snake_head[player] = 2;
	snake_count[player] = 3;
}


void snake_newGame() {
	uint8_t i;
	uint8_t playerdir[] = {EAST, WEST, WEST, EAST};
	
	//reset snake body
	
	frame.clear();
	snake_drawBorder();

	for (i = 0; i < playerCnt; i++) {
		//Player body init:
		uint8_t ctrl = player[i]; //Which controller is playing this player
		snake_startPos(ctrl, 7 + ((i == 1 || i == 2) ? 45 : 0), 15 + ((i < 2) ? 34 : 0), playerdir[i]);
		snake_reDraw(ctrl);
	}

	snake_newFood();
	
	frame.update();
	delay(2000);
	frame.resetButtons();
}


void snake_splash() {
  uint8_t y = 8;
  frame.clear();
  for (uint8_t y = 0; y < 6; y++) {
    for (uint8_t x = 0; x < 6; x++) {
      frame.blit(snake_splashscrn+x*16+y*96, 8+x*8, 8+y*8);
    }
  }
  frame.gotoXY(32, 16);
  frame.print("Snake", GREEN);
}


void snake_idle(uint8_t idle_counter) {
  if (idle_counter % 4 == 0) {
    if (idle_counter % 32 == 0) {
       //Move eyes every once in a while:
       frame.red[1][12] ^= 0x80;
       frame.green[1][12] ^= 0x80;
       frame.red[1][13] ^= 0x80;
       frame.green[1][13] ^= 0x80;
       
       frame.red[2][18] ^= 0x02;
       frame.green[2][18] ^= 0x02;
       frame.red[2][19] ^= 0x02;
       frame.green[2][19] ^= 0x02;
    }
    //Wag the snake's tongue quite often:
    frame.red[3][8] ^= 0x30;
    frame.red[3][9] ^= 0xF0;
  }
}


void snake_updateCornerScores() {
	//Displays the scores of each player in the corners of the board
	uint8_t i;
	char buff[5]; //Number to ascii buffer

	for (i = 0; i < playerCnt; i++) {
		switch(i) {
			case PLAYER1:
				frame.gotoXY(1,56);
				break;
			case PLAYER2:
				frame.gotoXY(52,56);
				break;
			case PLAYER3:
				frame.gotoXY(52,0);
				break;
			case PLAYER4:
				frame.gotoXY(1,0);
				break;
		}

		//Fetch score
		itoa((snake_count[player[i]]-3) >> 1, buff, 10);

		frame.clear(11);
		frame.print(buff, i < 2 ? GREEN : RED);
		/*if (i > 1) {
			//Flip score display of player 3 and 4
			menu_flipArea(0, i == PLAYER3 ? 52 : 1, 10, 3);
		}*/
	}
}


uint8_t snake_menu() { //HANDLED BY MENU INSTEAD?
	const int8_t x_coord[] = {5, 47, 5, 47};
	const int8_t y_coord[] = {40, 40, 16, 16};
	uint8_t count = 0;
	frame.clear();
	snake_splash();

	frame.resetButtons();
	frame.update();
	delay(1000);

	//TODO: Add actual menu here

	//Fetch players
	frame.clear();
	snake_drawBorder();
	menu_playerStart(1, 28, x_coord, y_coord);

	return 1;
}


void snake_run() {
	uint8_t p_alive[4], i, gameOn, isRunning;
	char buff[5]; //Number to ascii buffer
	frame.begin();
	isRunning = 1;

	while(isRunning) {
		//New game/exit menu here
		isRunning = snake_menu();
		
		if (isRunning) {
			snake_newGame();
			delayT = 150;
			gameOn = 1;

			for (i = 0; i < playerCnt; i++) {
				//if (player[i] != -1) {
				//Set all joined players to alive:
				p_alive[player[i]] = 1;
				//}
			}

			while(gameOn) { //Main game loop
				gameOn = 0; //A live player will set this back to 1
				
				//Execute all moves
				for (i = 0; i < playerCnt; i++) {
					if (p_alive[player[i]]) {
						snake_move(player[i]);
					}
				}

				//Check food
				for (i = 0; i < playerCnt; i++) {
					uint8_t ctrl = player[i];
					if (p_alive[ctrl] && (snake_headPosX[ctrl] == foodX) && (snake_headPosY[ctrl] == foodY)) {
						snake_newFood();
						//Clear old food so we don't collide with it....
						frame.setPixel(snake_headPosX[ctrl], snake_headPosY[ctrl], BLACK);
						//has eaten, update head, keep rest of snake stationary
						snake_addSeg(ctrl);
						snake_addSeg(ctrl);
					}
				}

				//Now that the foodcoloured pixels have been taken care of it's safe to assume
				//any remaining pixels are actual collisions.
				for (i = 0; i < playerCnt; i++) {
					uint8_t ctrl = player[i];
					if (p_alive[ctrl]) {
						p_alive[ctrl] = !snake_checkCollision(ctrl, p_alive);

						if (p_alive[ctrl]) {
							//Draw new head:
							frame.setPixel(snake_headPosX[ctrl], snake_headPosY[ctrl], (i & 1) ? ORANGE : (i < 2 ? GREEN : RED));
						}
						//Set old head to body colour: (will embed head in object if a collision happened)
						uint8_t prevX, prevY, prevDir;
						prevDir = snake_getLastDir(ctrl);
						prevX = snake_headPosX[ctrl] - snake_dirToX(prevDir);
						prevY = snake_headPosY[ctrl] - snake_dirToY(prevDir);

						frame.setPixel(prevX, prevY, (i < 2) ? GREEN : RED);
					}
					//Keep game running if one player is still alive:
					gameOn |= p_alive[ctrl];
				}
				food_timer++;
				if (food_timer > 96) {
					//clear current food:
					frame.setPixel(foodX, foodY, BLACK);
					//Make new:
					snake_newFood();
				}

				snake_updateCornerScores();

				frame.update();
				delay(delayT); //The timer2 interrrupt will ensure we still sample the player's D-pad while waiting
			}

			//Game over
			frame.resetButtons();
			delay(3000);

			while(!isStartPressed()) {
				//Display score summary here


				//Display result until both players press both buttons
				/*frame.clear();
				frame.gotoXY(7, 0);
				if (p1_alive == p2_alive) {
					//Both are dead		
					if (snake_count[PLAYER1] > snake_count[PLAYER2]) {
						frame.print("WIN", GREEN);	
					} else if (snake_count[PLAYER1] < snake_count[PLAYER2]) {
						frame.print("WIN", RED);
					} else {
						frame.gotoXY(5, 0);
						frame.print("DRAW", ORANGE);
					}
				} else if (p1_alive) {
					frame.print("WIN", GREEN);
				} else {
					frame.print("WIN", RED);
				}
				frame.update();
				if (isStartPressed()) {
					//Exit the scoreboard if the startbutton is held early
					break;
				}
				delay(1000);
				
				frame.clear();
				//display scores:
				itoa(snake_count[PLAYER1]-3, buff, 10);
				frame.gotoXY(1, 0);
				frame.print("P:", GREEN);
				frame.print(buff, GREEN);
				
				itoa(snake_count[PLAYER2]-3, buff, 10);
				frame.gotoXY(1, 8);
				frame.print("P:", RED);
				frame.print(buff, RED);
				
				frame.update();
				if (isStartPressed()) {
					//Exit the scoreboard if the startbutton is held early
					break;
				}
				delay(1000);*/
			}
		}
		isRunning = 0;	//CHANGE LATER ON when/if the menu is implemented
	}
}

