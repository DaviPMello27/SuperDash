#ifndef PLAYERSTRUCTS_H
#define PLAYERSTRUCTS_H

#include <SDL/SDL.h>
#include <iostream>
#include "screen.h"
#include "constants/map.h" //temporary until we build the maps using the Map struct
#include "player_components.h"
#include "structs/character.h"

namespace tools {
	static bool collide(SDL_Rect first, SDL_Rect second){
		if(first.x + first.w > second.x && first.x < second.x + second.w && first.y < second.y + second.h && first.y + first.h > second.y){
			return true;
		}
		return false;
	}

	static Direction getCollisionDirection(SDL_Point a, SDL_Point b, Size size){
		int xDif = a.x - b.x;
		int yDif = a.y - b.y;
		if(abs(xDif) + size.h > abs(yDif) + size.w + 1){
			if(xDif < 0){
				return Direction::RIGHT;
			} else {
				return Direction::LEFT;
			}
		} else {
			if(yDif < 0){
				return Direction::UP;
			} else {
				return Direction::DOWN;
			}
		}
	}
}

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
		this->direction = (x < 300) ? Direction::RIGHT : Direction::LEFT;
		this->speed = {0, 0};
		this->state = State::WALKING;
		this->keys = {0, 0, 0, 0};
		this->keyCodes = codes;
		this->pos = {x, y};
		this->size = {48, 76};
		this->animation = {AnimationType::WALK, 0, 0, {animation.offset, 0, 24, 38}, {pos.x, pos.y, size.w, size.h}};
		this->canJump = true;
		this->canDash = true;
		this->dashCooldown = 0;
		this->health = 1;
		this->team = 0;
	}

private: 
	void kill(float killerSpeed, bool knockback = true){
		state = State::DEFEATED;
		if((direction == Direction::LEFT && killerSpeed < 0) || (direction == Direction::RIGHT && killerSpeed > 0)){
			animation.type = AnimationType::DEATHBACK;
		} else {
			animation.type = AnimationType::DEATHFRONT;
		}
		if(knockback){
			speed.x = killerSpeed * 1.5f;
			pos.y -= 30;
			speed.y -= 10;
		}
	}

	void applyXSpeed(){
		if(abs(static_cast<int>(speed.x)) > 5){
			for(int i = 0; i < 2; i++){
				pos.x += static_cast<int>(speed.x) / 2;
				collideLeft();
				collideRight();
			}
		} else {
			pos.x += static_cast<int>(speed.x);
			collideLeft();
			collideRight();
		}
	}
	
	void applyYSpeed(){
		if(abs(static_cast<int>(speed.y)) > 5){
			for(int i = 0; i < 2; i++){
				pos.y += static_cast<int>(speed.y) / 2;
				collideUp();
				collideDown();
			}
		} else {
			pos.y += static_cast<int>(speed.y);
			collideUp();
			collideDown();
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
			if(state != State::DEFEATED){
				state = State::WALKING;
				animation.type = AnimationType::WALK;
			}
		}
	}

	void collideRight(){
		if(map1[pos.y / 30][((pos.x + size.w - 1) / 40) % 20] == 1 ||                     //top right  
			map1[(pos.y + size.h / 2) / 30][((pos.x + size.w - 1) / 40) % 20] == 1 ||     //middle right
			map1[(pos.y + size.h - 1) / 30][((pos.x + size.w - 1) / 40) % 20] == 1){      //bottom right
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void collideLeft(){
		if(map1[ pos.y    /     30       ][((pos.x) / 40 + 20) % 20] == 1 ||                                  //top left
		   map1[(pos.y + size.h / 2) / 30][((pos.x) / 40 + 20) % 20] == 1 ||                  //middle left
		   map1[(pos.y + size.h - 1) / 30][((pos.x) / 40 + 20) % 20] == 1){                   //bottom left
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}

	void collidePlayerHorizontal(Player &player){
		if(state == State::DASHING && player.state != State::DASHING){
			player.kill(this->speed.x);
		} else if(state == State::DASHING && player.state == State::DASHING){
			dashCooldown = 0;
			player.dashCooldown = 0;
		} else {
			pos.x -= static_cast<int>(speed.x);
			speed.x = 0;
		}
	}

	void collidePlayerUp(Player &player){
		if(state != State::DASHING && player.state == State::DASHING){
			kill(player.speed.x);
		} else if(state == State::DASHING && player.state == State::DASHING){
			dashCooldown = 0;
			player.dashCooldown = 0;
		} else { 
			speed.y = -10.0f;
			speed.x = (pos.x - player.pos.x) / 5.0f;
			player.dashCooldown = 0;
			dashCooldown = 0;
		}
	}

	void collidePlayerDown(Player &player){
		if(state != State::DASHING && player.state == State::DASHING){
			kill(player.speed.x);
		} else if(state == State::DASHING && player.state == State::DASHING){
			dashCooldown = 0;
			player.dashCooldown = 0;
		} else {
			speed.y = 0;
			player.dashCooldown = 0;
			dashCooldown = 0;
		}
	}

	void checkPlayersCollision(Player &player){
		if(abs(pos.x - player.pos.x) < size.w && abs(pos.y - player.pos.y) < size.h){ //check if colliding
			Direction dir = tools::getCollisionDirection(pos, player.pos, player.size);
			if((dir == Direction::RIGHT || dir == Direction::LEFT)){
				collidePlayerHorizontal(player);
			} else if(dir == Direction::UP){
				collidePlayerUp(player);
			} else if(dir == Direction::DOWN){
				collidePlayerDown(player);
			}
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
				animation.type = AnimationType::JUMP;
			}
			speed.y -= 4 + (0.4f*character->stat.jumpPower);
			if(speed.y <= - (10 + (0.4f * character->stat.jumpPower))){
				canJump = false;
			}
		}

		//==========GRAVITY AND MIDAIR COLLISIONS==========//
		if(state == State::MIDAIR || state == State::DEFEATED){
			if(speed.y < 0 && !keys.up){
				speed.y += 0.5;
			}
			speed.y += 1.0f / 3;
			applyYSpeed();
			if(speed.y > 0) canJump = false;	
		}
		
		//==========RIGHT==========//
		if(keys.right && !keys.left){
			//speed increase
			if(speed.x < maxSpeed && state != State::DEFEATED){
				speed.x++;
			}
		} else if(speed.x > 0){
			speed.x -= 0.5;
		}

		//==========LEFT==========//
		if(keys.left && !keys.right){
			//speed increase
			if(speed.x > -maxSpeed && state != State::DEFEATED){
				speed.x--;
			}
		} else if(speed.x < 0){
			speed.x += 0.5;
		}
		applyXSpeed();

		//==========LANDING==========//
		if(state == State::WALKING &&
			map1[(pos.y + size.h+1) / 30][(pos.x) / 40] == 0 &&                    //bottom left
			map1[(pos.y + size.h+1) / 30][(pos.x + size.w) / 40] == 0){                //bottom right
			state = State::MIDAIR;
			animation.type = AnimationType::JUMP;
		}

		if(pos.x > 800){ //solve repetition on dash()
			pos.x = -size.w;
		} else if(pos.x < -size.w){
			pos.x = 800;
		}
	}

	void dash(){
		float maxSpeed = 4.0f + character->stat.dashSpeed;
		if(!dashCooldown){
			animation.type = AnimationType::JUMP;
			state = State::MIDAIR;
			dashCooldown = (6 - character->stat.recoveryTime) * 30;
			size.h = 76;
			return;
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

		if(pos.x > 800){
			pos.x = -size.w;
		} else if(pos.x < -size.w){
			pos.x = 800;
		}
	}

public:
	void control(){
		if(dashCooldown){dashCooldown--;}
		if(state != State::DASHING){
			move();
		} else if(state == State::DASHING){
			dash();
		}
		if(animation.type == AnimationType::WALK){
			animation.walk((keys.right || keys.left), 168, 24);
		} else if(animation.type == AnimationType::JUMP){
			animation.jump(speed.y);
		} else if(animation.type == AnimationType::DEATHBACK){
			animation.death(speed.y, 96);
		} else if(animation.type == AnimationType::DEATHFRONT){
			animation.death(speed.y);
		} else if(animation.type == AnimationType::DASH){
			animation.dash(speed.x, 4.0 + character->stat.dashSpeed);
		}
	}

	void collidePlayers(Player* players){
		for(int i = 0; i < 2; i++){ //check all players
			if(players[i].state != State::DEFEATED && state != State::DEFEATED){ //check if players are alive
				if(pos.x != players[i].pos.x || pos.y != players[i].pos.y){ //check if player's not himself
					checkPlayersCollision(players[i]);
				}
			}
		}
	}

	void draw(SDL_Renderer* renderer){ //organize this mess
		if(animation.type == AnimationType::DASH && direction == Direction::RIGHT){
			animation.dst = {pos.x - 106, pos.y, 144, size.h};
		} else if(animation.type == AnimationType::DASH && direction == Direction::LEFT){
			animation.dst = {pos.x, pos.y, 144, size.h};
		} else if(animation.type == AnimationType::DASH && direction == Direction::UP){
			animation.dst = {pos.x - size.w, pos.y + size.h, 144, size.h};
			SDL_RenderCopyEx(renderer, character->sprite, &animation.src, &animation.dst, 270, {0}, SDL_FLIP_NONE);
			animation.dst = {pos.x, pos.y, size.w, 144};
			return;
		} else if(animation.type == AnimationType::DASH && direction == Direction::DOWN){
			animation.dst = {pos.x - size.w, pos.y + size.h, 144, size.h};
			SDL_RenderCopyEx(renderer, character->sprite, &animation.src, &animation.dst, 90, {0}, SDL_FLIP_NONE);
			animation.dst = {pos.x, pos.y, size.w, 144};
			return;
		} else {
			animation.dst = {pos.x, pos.y, size.w, size.h};
		}
		
		if(direction == Direction::LEFT){
			SDL_RenderCopyEx(renderer, character->sprite, &animation.src, &animation.dst, 0, {0}, SDL_FLIP_HORIZONTAL);
		} else {
			SDL_RenderCopyEx(renderer, character->sprite, &animation.src, &animation.dst, 0, {0}, SDL_FLIP_NONE);
		}
	}

	void respondToKey(SDL_Event event){
		SDL_Keycode key = event.key.keysym.sym;
		switch(event.type){
			case SDL_KEYDOWN:
				if(state != State::DEFEATED){
					if(key == keyCodes.right){
						keys.right = true;
						if(state != State::DASHING){direction = Direction::RIGHT;}
					} else if(key == keyCodes.left){
						keys.left = true;
						if(state != State::DASHING){direction = Direction::LEFT;}
					} else if(key == keyCodes.up){
						keys.up = true;
					} else if(key == keyCodes.down){
						keys.down = true;
					} else if(key == keyCodes.dash && state != State::DASHING && !dashCooldown){
						if(keys.left || keys.up || keys.right || keys.down){
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
							animation.type = AnimationType::DASH;
							animation.counter = 0;
							state = State::DASHING;
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