#ifndef PLAYERSTRUCTS_H
#define PLAYERSTRUCTS_H

#include <SDL/SDL.h>
#include "screen.h"
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
	Size size;
	bool canJump = true;
	bool canDash = true;
	int dashCooldown;
	int health;
	int team;

	Player(const Character* character, int x, int y, KeyCodes codes){
		this->character = character;
		this->direction = (x < 300) ? Direction::RIGHT : Direction::LEFT;
		this->speed = {0, 0};
		this->state = State::WALKING;
		this->keys = {0, 0, 0, 0};
		this->keyCodes = codes;
		this->pos = {x, y};
		this->size = {20, 50};
		this->canJump = true;
		this->canDash = true;
		this->dashCooldown = 0;
		this->health = 1;
		this->team = 0;
	}

	void control(){
		if(state != State::DASHING){
			move();
		} else if(state == State::DASHING){
			dash();
		}
	}

	void collideUp(){
		if(
			speed.y < 0 &&                                               //if character is moving up
			(map1[(pos.y) / 30][(pos.x) / 40] == 1 ||                    //if top-left is hitting block
				map1[(pos.y) / 30][(pos.x + size.w - 1) / 40] == 1)              //if top-right is hitting block
			){
			pos.y += 30 - (pos.y) % 30; //set position to the bottom of the block
			speed.y = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void collideDown(){
		if(map1[(pos.y + size.h) / 30][(pos.x) / 40] == 1 ||                //bottom left
			map1[(pos.y + size.h) / 30][(pos.x + size.w - 1) / 40] == 1){            //bottom right
			pos.y -= (pos.y + size.h) % 30; //set position to the top of the block
			speed.y = 0;
			if(state == State::DASHING){dashCooldown = 0;}
			state = State::WALKING;
		}
	}

	void collideRight(){
		if(map1[pos.y / 30][(pos.x + size.w - 1) / 40] == 1 ||                   //top right  
			map1[(pos.y + size.h / 2) / 30][(pos.x + size.w - 1) / 40] == 1 ||           //middle right
			map1[(pos.y + size.h - 1) / 30][(pos.x + size.w - 1) / 40] == 1){            //bottom right
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void collideLeft(){
		if(map1[pos.y / 30][(pos.x) / 40] == 1 ||                        //top left
			map1[(pos.y + size.h / 2) / 30][(pos.x) / 40] == 1 ||                //middle left
			map1[(pos.y + size.h - 1) / 30][(pos.x) / 40] == 1){                 //bottom left
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void move(){
		float maxSpeed = 2.0f + character->stat.speed;
		if(state == State::MIDAIR){
			maxSpeed /= 1.5;
		}

		if(state == State::WALKING && keys.up == false){
			canJump = true;
		}
		
		//==========JUMP==========//
		if(keys.up && canJump){
			if(state == State::WALKING){
				pos.y--;
				state = State::MIDAIR;
			}
			speed.y -= 4 + (0.4*character->stat.jumpPower);
			if(speed.y <= - (10 + (0.4 * character->stat.jumpPower))){
				canJump = false;
			}
		}

		//==========MIDAIR==========//
		if(state == State::MIDAIR){
			if(speed.y < 0 && !keys.up){
				speed.y += 0.5;
			}
			speed.y += 1.0f / 3;

			if(speed.y > 0) canJump = false;	

			pos.y += static_cast<int>(speed.y);

			//up collision detection
			collideUp();
			collideDown();
		}
		
		//==========RIGHT==========//
		if(keys.right){
			//speed increase
			if(speed.x < maxSpeed){
				speed.x++;
			}
			pos.x += static_cast<int>(speed.x);
			collideRight();
		} else if(speed.x > 0){
			speed.x = 0;
		}

		//==========LEFT==========//
		if(keys.left){
			//speed increase
			if(speed.x > -maxSpeed){
				speed.x--;
			}
			pos.x += static_cast<int>(speed.x);
			collideLeft();
		} else if(speed.x < 0){
			speed.x = 0;
		}

		//==========LANDING==========//
		if(state == State::WALKING &&
			map1[(pos.y + size.h+1) / 30][(pos.x) / 40] == 0 &&                    //bottom left
			map1[(pos.y + size.h+1) / 30][(pos.x + 20) / 40] == 0){                //bottom right
			state = State::MIDAIR;
		}
	}

	void dash(){
		float maxSpeed = 4.0f + character->stat.dashSpeed;
		if(!dashCooldown){
			state = State::MIDAIR;
			size.h = 50;
			speed = {0, 0};
		}
		dashCooldown--;

		//==========MIDAIR==========//
		//up collision detection
		if(
			speed.y < 0 &&                                               //if character is moving up
			(map1[(pos.y) / 30][(pos.x) / 40] == 1 ||                    //if top-left is hitting block
				map1[(pos.y) / 30][(pos.x + size.w - 1) / 40] == 1)              //if top-right is hitting block
			){
			pos.y += 30 - (pos.y) % 30; //set position to the bottom of the block
			speed.y = 0;
			dashCooldown = 0;
		}

		//left and right collision
		if(map1[(pos.y + 0) / 30][(pos.x) / 40] == 1 ||                  //top left
			map1[(pos.y + 0) / 30][(pos.x + size.w - 1) / 40] == 1 ||            //top right
			map1[(pos.y + size.h / 2) / 30][(pos.x) / 40] == 1 ||                //middle left
			map1[(pos.y + size.h / 2) / 30][(pos.x + size.w - 1) / 40] == 1 ||           //middle right
			map1[(pos.y + size.h) / 30][(pos.x) / 40] == 1 ||                //bottom left
			map1[(pos.y + size.h) / 30][(pos.x + size.w - 1) / 40] == 1){            //bottom right
			pos.y -= (pos.y + size.h) % 30; //set position to the top of the block
			speed.y = 0;
			dashCooldown = 0;
			state = State::WALKING;
		}

		//down collision//
		if(state == State::WALKING &&
			map1[(pos.y + size.h + 1) / 30][(pos.x) / 40] == 0 &&                    //bottom left
			map1[(pos.y + size.h + 1) / 30][(pos.x + 20) / 40] == 0){                //bottom right
			state = State::MIDAIR;
			dashCooldown = 0;
		}

		if(state == State::DASHING){
			//==========RIGHT==========//
			if(direction == Direction::RIGHT){
				//speed increase
				if(speed.x < maxSpeed){
					speed.x++;
				}
				pos.x += static_cast<int>(speed.x);
				//right collision detection
				if(map1[pos.y / 30][(pos.x + size.w - 1) / 40] == 1 ||                   //top right  
					map1[(pos.y + size.h / 2) / 30][(pos.x + size.w - 1) / 40] == 1 ||           //middle right
					map1[(pos.y + size.h - 1) / 30][(pos.x + size.w - 1) / 40] == 1){            //bottom right
					pos.x -= static_cast<int>(speed.x);
					speed.x = 0;
					dashCooldown = 0;
				}
			} else if(speed.x > 0){
				speed.x--;
			}

			//==========LEFT==========//
			if(direction == Direction::LEFT){
				//speed increase
				if(speed.x > -maxSpeed){
					speed.x--;
				}
				pos.x += static_cast<int>(speed.x);
				//left collision detection
				if(map1[pos.y / 30][(pos.x) / 40] == 1 ||                        //top left
					map1[(pos.y + size.h / 2) / 30][(pos.x) / 40] == 1 ||                //middle left
					map1[(pos.y + size.h - 1) / 30][(pos.x) / 40] == 1){                 //bottom left
					pos.x -= static_cast<int>(speed.x);
					speed.x = 0;
					dashCooldown = 0;
				}
			} else if(speed.x < 0){
				speed.x++;
			}
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
				else if(key == SDLK_SPACE && state != State::DASHING){
					if(keys.left || keys.up || keys.right || keys.down){
						state = State::DASHING;
						dashCooldown = 30;
						size.h = 20;
						if(keys.left){
							direction = Direction::LEFT;
						} else if(keys.right){
							direction = Direction::RIGHT;
						} else if(keys.up){
							direction = Direction::UP;
						} else if(keys.down){
							direction = Direction::DOWN;
						}
					}
				}
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
