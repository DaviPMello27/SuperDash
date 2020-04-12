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

struct KeyCodes {
	SDL_Keycode left;
	SDL_Keycode up;
	SDL_Keycode right;
	SDL_Keycode down;
};

enum class Direction{
	LEFT,
	UP,
	RIGHT,
	DOWN,
};

enum class State {
	WALKING,
	DASHING,
	MIDAIR,
	DEFEATED,
};

struct Speed {
	float x;
	float y;
};

struct Player {
	const Character* character;
	Direction direction;
	Speed speed;
	State state;
	DirectionKeys keys;
	KeyCodes keyCodes;
	SDL_Point pos;
	bool canJump = true;
	int dashCooldown;
	int health;
	int team;

	void move(){
		float maxSpeed = 2.0f + character->stat.speed; //temporary until we set maxSpeed based on character data
		if(state == State::MIDAIR){
			maxSpeed /= 1.5;
			canJump = false;
		}

		if(state == State::WALKING && keys.up == false){
			canJump = true;
		}
		
		//==========JUMP==========//
		if(keys.up && canJump){
			if(state == State::WALKING){
				speed.y = -15;
				pos.y--;
				state = State::MIDAIR;
			}
		}

		//==========MIDAIR==========//
		if(state == State::MIDAIR){
			speed.y += 1.0f / 3;

			pos.y += static_cast<int>(speed.y);

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
				state = State::WALKING;
			}
		}
		
		//==========RIGHT==========//
		if(keys.right){
			//speed increase
			if(speed.x < maxSpeed){
				speed.x++;
			}
			pos.x += static_cast<int>(speed.x);
			//right collision detection
			if(map1[pos.y / 30][(pos.x + 19) / 40] == 1 ||                   //top right  
				map1[(pos.y + 25) / 30][(pos.x + 19) / 40] == 1 ||           //middle right
				map1[(pos.y + 49) / 30][(pos.x + 19) / 40] == 1){            //bottom right
				pos.x -= static_cast<int>(speed.x);
				speed.x = 0;
			}
		} else if(speed.x > 0){
			speed.x--;
		}

		//==========LEFT==========//
		if(keys.left){
			//speed increase
			if(speed.x > -maxSpeed){
				speed.x--;
			}
			pos.x += static_cast<int>(speed.x);
			//left collision detection
			if(map1[pos.y / 30][(pos.x) / 40] == 1 ||                        //top left
				map1[(pos.y + 25) / 30][(pos.x) / 40] == 1 ||                //middle left
				map1[(pos.y + 49) / 30][(pos.x) / 40] == 1){                 //bottom left
				pos.x -= static_cast<int>(speed.x);
				speed.x = 0;
			}
		} else if(speed.x < 0){
			speed.x++;
		}

		//==========LANDING==========//
		if(state == State::WALKING &&
			map1[(pos.y + 51) / 30][(pos.x) / 40] == 0 &&                    //bottom left
			map1[(pos.y + 51) / 30][(pos.x + 20) / 40] == 0){                //bottom right
			state = State::MIDAIR;
		}
	}

	void respondToKey(SDL_Event event){
		SDL_Keycode key = event.key.keysym.sym;
		switch(event.type){
			case SDL_KEYDOWN:
				if(key == keyCodes.right)
					keys.right = true;
				else if(key == keyCodes.left)
					keys.left = true;
				else if(key == keyCodes.up)
					keys.up = true;
				else if(key == keyCodes.down)
					keys.down = true;
			break;
			case SDL_KEYUP:
				if(key == keyCodes.right)
					keys.right = false;
				else if(key == keyCodes.left)
					keys.left = false;
				else if(key == keyCodes.up)
					keys.up = false;
				else if(key == keyCodes.down)
					keys.down = false;
			break;
		}
	}
};

#endif
