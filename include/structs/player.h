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
	SDL_Keycode dash;
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

struct Animation {
	int counter;
	int offset;
	SDL_Rect src;

	void walk(bool condition, int limit, int variation){
		src = {offset, 0, 24, 38};
		if(condition){
			counter++;
			if(counter % 10 == 0){
				offset += variation;
			}
			if(offset == limit){
				offset = variation;
			}
		} else {
			counter = 0;
			offset = 0;
		}
	}

	void jump(int ySpeed, int limit, int variation){
		if(ySpeed < -1.5){
			offset = 168;
		} else if(ySpeed >= -1.5 && ySpeed < 1.5){
			offset = 192;
		} else {
			offset = 216;
		}
		src = {offset, 0, 24, 38};
	}
};

struct Player {
	Character* character;
	Animation animation;
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

	Player(Character* character, int x, int y, KeyCodes codes){
		this->character = character;
		this->animation = {0, 0, {animation.offset, 0, 24, 38}};
		this->direction = (x < 300) ? Direction::RIGHT : Direction::LEFT;
		this->speed = {0, 0};
		this->state = State::WALKING;
		this->keys = {0, 0, 0, 0};
		this->keyCodes = codes;
		this->pos = {x, y};
		this->size = {48, 76};
		this->canJump = true;
		this->canDash = true;
		this->dashCooldown = 0;
		this->health = 1;
		this->team = 0;
	}

	void control(){
		if(dashCooldown){dashCooldown--;}
		if(state != State::DASHING){
			move();
			if(state == State::MIDAIR)
				animation.jump(speed.y, 192, 24);
			else
				animation.walk((keys.right || keys.left), 168, 24);
		} else if(state == State::DASHING){
			dash();
		}
	}

	void collideUp(){
		if(speed.y < 0 &&                                                          //if character is moving up
		  (map1[(pos.y) / 30][(pos.x) / 40] == 1 ||                                //if top-left is hitting block
		   map1[(pos.y) / 30][(pos.x + size.w - 1) / 40] == 1)                     //if top-right is hitting block
		){
			pos.y += 30 - (pos.y) % 30; //set position to the bottom of the block
			speed.y = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void collideDown(){
		if(speed.y > 0 && 
		   (map1[(pos.y + size.h) / 30][(pos.x) / 40] == 1 ||                       //bottom left
			map1[(pos.y + size.h) / 30][(pos.x + size.w - 1) / 40] == 1)){          //bottom right
			pos.y -= (pos.y + size.h) % 30; //set position to the top of the block
			speed.y = 0;
			animation.offset = 0;
			if(state == State::DASHING){
				pos.y -= 30;
				dashCooldown = 0;
				return;
			}
			state = State::WALKING;
		}
	}

	void collideRight(){
		if(map1[pos.y / 30][(pos.x + size.w - 1) / 40] == 1 ||                     //top right  
			map1[(pos.y + size.h / 2) / 30][(pos.x + size.w - 1) / 40] == 1 ||     //middle right
			map1[(pos.y + size.h - 1) / 30][(pos.x + size.w - 1) / 40] == 1){      //bottom right
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void collideLeft(){
		if(map1[ pos.y    /     30       ][(pos.x) / 40] == 1 ||                                  //top left
		   map1[(pos.y + size.h / 2) / 30][(pos.x) / 40] == 1 ||                  //middle left
		   map1[(pos.y + size.h - 1) / 30][(pos.x) / 40] == 1){                   //bottom left
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void move(){
		float maxSpeed = 2.0f + character->stat.speed;
		if(state == State::MIDAIR)
			maxSpeed /= 1.5;
		
		if(state == State::WALKING && keys.up == false)
			canJump = true;
		
		//==========JUMP==========//
		if(keys.up && canJump){
			if(state == State::WALKING){
				pos.y--;
				state = State::MIDAIR;
			}
			speed.y -= 4 + (0.4f*character->stat.jumpPower);
			if(speed.y <= - (10 + (0.4f * character->stat.jumpPower))){
				canJump = false;
			}
		}

		//==========GRAVITY AND MIDAIR COLLISIONS==========//
		if(state == State::MIDAIR){
			if(speed.y < 0 && !keys.up){
				speed.y += 0.5;
			}
			speed.y += 1.0f / 3;
			pos.y += static_cast<int>(speed.y);
			if(speed.y > 0) canJump = false;	
			collideUp();
			collideDown();
		}
		
		//==========RIGHT==========//
		if(keys.right && !keys.left){
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
		if(keys.left && !keys.right){
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
			dashCooldown = (6 - character->stat.recoveryTime) * 30;
			size.h = 76;
			speed = {0, 0};
		}

		if(state == State::DASHING){
			switch(direction){
				case Direction::UP:
					if(speed.y > -maxSpeed){
						speed.y -= 3 + character->stat.dashSpeed / 3;
					}
					pos.y += static_cast<int>(speed.y);
					collideUp();
				break;
				case Direction::DOWN:
					if(speed.y < maxSpeed){
						speed.y += 3 + character->stat.dashSpeed;
					}
					pos.y += static_cast<int>(speed.y);
					collideDown();
				break;
				case Direction::RIGHT:
					if(speed.x < maxSpeed){
						speed.x += 3 + character->stat.dashSpeed / 3;
					}
					pos.x += static_cast<int>(speed.x);
					collideRight();
				break;
				case Direction::LEFT:
					if(speed.x > -maxSpeed){
						speed.x -= 3 + character->stat.dashSpeed / 3;
					}
					pos.x += static_cast<int>(speed.x);
					collideLeft();
				break;
			}
		}
	}

	

	void respondToKey(SDL_Event event){
		SDL_Keycode key = event.key.keysym.sym;
		switch(event.type){
			case SDL_KEYDOWN:
				if(key == keyCodes.right){
					keys.right = true;
					direction = Direction::RIGHT;
				} else if(key == keyCodes.left){
					keys.left = true;
					direction = Direction::LEFT;
				} else if(key == keyCodes.up){
					keys.up = true;
				} else if(key == keyCodes.down){
					keys.down = true;
				} else if(key == keyCodes.dash && state != State::DASHING && !dashCooldown){
					if(keys.left || keys.up || keys.right || keys.down){
						state = State::DASHING;
						dashCooldown = 30;
						size.h = 48;
						speed = {0, 0};
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