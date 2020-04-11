#ifndef PLAYERSTRUCTS_H
#define PLAYERSTRUCTS_H

#include <SDL/SDL.h>
#include "headers.h"
#include "constants/map.h" //temporary until we build the maps using the Map struct

struct DirectionKeys {
	bool left;
	bool up;
	bool right;
	bool down;
};

enum Direction{
	DIRECTION_LEFT,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
};

enum State {
	STATE_WALKING,
	STATE_DASHING,
	STATE_MIDAIR,
	STATE_DEFEATED,
};

struct Speed {
	float x;
	float y;
};

struct Player {
	Character* character;
	Direction direction;
	Speed speed;
	State state;
	DirectionKeys keys;
	SDL_Point pos;
	int dashRecovery;
	int health;
	int team;

	void move(){ //we'll fix the float to int conversions later
		float maxSpeed = 4; //temporary until we set maxSpeed based on character data
		if(state == STATE_MIDAIR){
			maxSpeed /= 1.5;
		}
		//jump
		if(keys.up){
			if(state == STATE_WALKING){
				speed.y = -15;
				pos.y--;
				state = STATE_MIDAIR;
			}
		}

		//collision mid air
		if(state == STATE_MIDAIR){
			speed.y += 1.0 / 3;

			pos.y += speed.y;

			//up collision detection
			if(
				speed.y < 0 &&                                               //if character is moving up
				(map1[(pos.y) / 30][(pos.x) / 40] == 1 ||                    //if top-left is hitting block
					map1[(pos.y) / 30][(pos.x + 19) / 40] == 1)              //if top-right is hitting block
				){
				pos.y += 30 - (pos.y) % 30; //set position to the bottom of the block
				speed.y = 0;
			}

			if(map1[(pos.y + 0) / 30][(pos.x) / 40] == 1 ||                  //top left
				map1[(pos.y + 0) / 30][(pos.x + 19) / 40] == 1 ||            //top right
				map1[(pos.y + 25) / 30][(pos.x) / 40] == 1 ||                //middle left
				map1[(pos.y + 25) / 30][(pos.x + 19) / 40] == 1 ||           //middle right
				map1[(pos.y + 50) / 30][(pos.x) / 40] == 1 ||                //bottom left
				map1[(pos.y + 50) / 30][(pos.x + 19) / 40] == 1){            //bottom right
				pos.y -= (pos.y + 50) % 30; //set position to the top of the block
				speed.y = 0;
				state = STATE_WALKING;
			}
		}

		if(keys.right){
			//speed increase
			if(speed.x < maxSpeed){
				speed.x++;
			}
			pos.x += speed.x;
			//right collision detection
			if(map1[pos.y / 30][(pos.x + 19) / 40] == 1 ||                   //top right  
				map1[(pos.y + 25) / 30][(pos.x + 19) / 40] == 1 ||           //middle right
				map1[(pos.y + 49) / 30][(pos.x + 19) / 40] == 1){            //bottom right
				pos.x -= speed.x;
				speed.x = 0;
			}
		} else if(speed.x > 0){
			speed.x--;
		}

		if(keys.left){
			//speed increase
			if(speed.x > -maxSpeed){
				speed.x--;
			}
			pos.x += speed.x;
			//left collision detection
			if(map1[pos.y / 30][(pos.x) / 40] == 1 ||                        //top left
				map1[(pos.y + 25) / 30][(pos.x) / 40] == 1 ||                //middle left
				map1[(pos.y + 49) / 30][(pos.x) / 40] == 1){                 //bottom left
				pos.x -= speed.x;
				speed.x = 0;
			}
		} else if(speed.x < 0){
			speed.x++;
		}

		if(state == STATE_WALKING &&
			map1[(pos.y + 51) / 30][(pos.x) / 40] == 0 &&                    //bottom left
			map1[(pos.y + 51) / 30][(pos.x + 20) / 40] == 0){                //bottom right
			state = STATE_MIDAIR;
		}
	}
};

#endif
