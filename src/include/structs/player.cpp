#include "structs/player.h"

bool tools::collide(SDL_Rect first, SDL_Rect second) {
	if (first.x + first.w > second.x && first.x < second.x + second.w && first.y < second.y + second.h && first.y + first.h > second.y) {
		return true;
	}
	return false;
}

Direction tools::getCollisionDirection(SDL_Point a, SDL_Point b, Size size) {
	int xDif = a.x - b.x;
	int yDif = a.y - b.y;
	if (abs(xDif) + size.h > abs(yDif) + size.w + 1) {
		return (xDif < 0) ? Direction::RIGHT : Direction::LEFT;
	} else {
		return (yDif < 0) ? Direction::UP : Direction::DOWN;
	}
}


Player::Player(Character* character, int x, int y, KeyCodes codes){
	this->character = character;
	this->direction = (x < 300) ? Direction::RIGHT : Direction::LEFT;
	this->speed = {0, 0};
	this->state = State::WALKING;
	this->keys = {0, 0, 0, 0};
	this->keyCodes = codes;
	this->pos = {x, y};
	this->size = {25, 50};
	this->animation = {AnimationType::WALK, 0, 0, {animation.offset, 0, 24, 48}, {pos.x, pos.y, size.w, size.h}};
	this->canJump = true;
	this->canDash = true;
	this->dashCooldown = 0;
	this->health = 1;
	this->team = 0;
}

void Player::kill(float killerSpeed, bool knockback){
	state = State::DEFEATED;
	bool hitFromBehind = (direction == Direction::LEFT && killerSpeed < 0) || (direction == Direction::RIGHT && killerSpeed > 0);
	if (hitFromBehind) {
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

void Player::applyXSpeed(Map map) {
	if (abs(static_cast<int>(speed.x)) > 5) {
		for (int i = 0; i < 2; i++) {
			pos.x += static_cast<int>(speed.x) / 2;
			collideLeft(map);
			collideRight(map);
		}
	} else {
		pos.x += static_cast<int>(speed.x);
		collideLeft(map);
		collideRight(map);
	}
}

void Player::applyYSpeed(Map map) {
	if(abs(static_cast<int>(speed.y)) > 5){
		for(int i = 0; i < 2; i++){
			pos.y += static_cast<int>(speed.y) / 2;
			collideUp(map);
			collideDown(map);
		}
	} else {
		pos.y += static_cast<int>(speed.y);
		collideUp(map);
		collideDown(map);
	}
}

void Player::changeDirection() {
	if(keys.right){
		direction = Direction::RIGHT;
	} else if (keys.left){
		direction = Direction::LEFT;
	}
}

void Player::checkPacmanEffect(){ //TODO: solve repetition on dash()
	if(pos.x > 800){ //horizontal
		pos.x = -size.w;
	} else if(pos.x < -size.w) {
		pos.x = 800;
	}
	if(pos.y > 600){ //vertical
		pos.y = -size.h;
	} else if(pos.y < -size.h) {
		pos.y = 800;
	}
}

void Player::move(Map map) {
	float maxSpeed = 2.0f + character->stat.speed;
	if(state == State::MIDAIR)
		maxSpeed /= 1.5;

	if(state == State::WALKING && keys.up == false)
		canJump = true;

	changeDirection();

	//==========JUMP==========//
	if (keys.up && canJump) {
		if(state == State::WALKING){
			pos.y--;
			state = State::MIDAIR;
			animation.type = AnimationType::JUMP;
		}
		speed.y -= 4 + (0.4f * character->stat.jumpPower);
		if(speed.y <= -(8 + (0.4f * character->stat.jumpPower))){ //jump limit
			canJump = false;
		}
	}

	//==========GRAVITY AND MIDAIR COLLISIONS==========//
	if (state == State::MIDAIR || state == State::DEFEATED) {
		if (speed.y < 0 && !keys.up) {
			speed.y += 0.5;
		}
		speed.y += 1.0f / 5;
		applyYSpeed(map);
		if(speed.y > 0) canJump = false;
	}

	//==========RIGHT==========//
	if (keys.right && !keys.left) {
		//speed increase
		if (speed.x < maxSpeed && state != State::DEFEATED) {
			speed.x++;
		}
	} else if (speed.x > 0) {
		speed.x -= 0.5;
	}

	//==========LEFT==========//
	if (keys.left && !keys.right) {
		//speed increase
		if (speed.x > -maxSpeed && state != State::DEFEATED) {
			speed.x--;
		}
	} else if (speed.x < 0) {
		speed.x += 0.5;
	}

	applyXSpeed(map);

	//==========FALLING OFF==========//
	if(state == State::WALKING){
		char bottomLeft = map.tileData[(pos.y + size.h + 1) / 25][(pos.x) / 25];
		char bottomRight = map.tileData[(pos.y + size.h + 1) / 25][(pos.x + size.w) / 25];

		if(bottomLeft == 97 && bottomRight == 97){
			state = State::MIDAIR;
			animation.type = AnimationType::JUMP;
		}
	}
	checkPacmanEffect(); //remove from both move() and dash()?
}

void Player::dash(Map map) {
	float maxSpeed = 4.0f + character->stat.dashSpeed;
	if(!dashCooldown) {
		animation.type = AnimationType::JUMP;
		state = State::MIDAIR;
		canJump = false;
		dashCooldown = (6 - character->stat.recoveryTime) * 30;
		size.h = 50;
		return;
	}

	if (state == State::DASHING) {
		switch (direction) {
		case Direction::UP:
			if (speed.y > -maxSpeed) {
				speed.y -= 3 + character->stat.dashSpeed / 3;
			}
		break;
		case Direction::DOWN:
			if (speed.y < maxSpeed) {
				speed.y += 3 + character->stat.dashSpeed;
			}
		break;
		case Direction::RIGHT:
			if (speed.x < maxSpeed) {
				speed.x += 3 + character->stat.dashSpeed / 3;
			}
		break;
		case Direction::LEFT:
			if (speed.x > -maxSpeed) {
				speed.x -= 3 + character->stat.dashSpeed / 3;
			}
		break;
		}
		applyXSpeed(map);
		applyYSpeed(map);
	}

	checkPacmanEffect(); //remove from both move() and dash()?
}


// PUBLIC:
void Player::control(Map map) {
	if(dashCooldown){dashCooldown--;}
	if (state != State::DASHING) {
		move(map);
	} else if (state == State::DASHING) {
		dash(map);
	}
	switch(animation.type){
		case AnimationType::WALK:
			animation.walk((keys.right || keys.left), 168, 24); break;
		case AnimationType::JUMP:
			animation.jump(speed.y); break;
		case AnimationType::DEATHBACK:
			animation.death(speed.y, 96); break;
		case AnimationType::DEATHFRONT:
			animation.death(speed.y); break;
		case AnimationType::DASH:
			animation.dash(dashCooldown, 4.0 + character->stat.dashSpeed); break;
	}
}

void Player::collidePlayers(Player* players) {
	for (int i = 0; i < 2; i++) { //check all players
		if (players[i].state != State::DEFEATED && state != State::DEFEATED) { //check if players are alive
			if (pos.x != players[i].pos.x || pos.y != players[i].pos.y) { //check if player's not himself
				checkPlayersCollision(players[i]);
			}
		}
	}
}

void Player::draw(SDL_Renderer* renderer){
	double angle = 0;
	int yOffset = 0;
	int xOffset = 0;
	if(animation.type == AnimationType::DASH){
		switch(direction){
			case Direction::RIGHT:
				xOffset = -106;
			break;
			case Direction::UP:
				xOffset = -size.w * 1.5;
				yOffset = size.h;
				angle = 270;
			break;
			case Direction::DOWN:
				xOffset = -size.w * 1.5;
				yOffset = -size.h;
				angle = 90;
			break;
		}
		animation.dst = {pos.x + xOffset, pos.y + yOffset, 144, size.h};
	} else {
		animation.dst = {pos.x, pos.y, size.w, size.h};
	}
	
	if(direction == Direction::LEFT){
		SDL_RenderCopyEx(renderer, character->sprite, &animation.src, &animation.dst, angle, {0}, SDL_FLIP_HORIZONTAL);
	} else {
		SDL_RenderCopyEx(renderer, character->sprite, &animation.src, &animation.dst, angle, {0}, SDL_FLIP_NONE);
	}
	animation.dst = {pos.x, pos.y, size.w, size.h};
}

void Player::respondToKey(SDL_Event event) {
	SDL_Keycode key = event.key.keysym.sym;
	switch (event.type) {
	case SDL_KEYDOWN:
		if (state != State::DEFEATED) {
			if (key == keyCodes.right) {
				keys.right = true;
			} else if (key == keyCodes.left) {
				keys.left = true;
			} else if (key == keyCodes.up) {
				keys.up = true;
			} else if (key == keyCodes.down) {
				keys.down = true;
			} else if (key == keyCodes.dash && state != State::DASHING && !dashCooldown) {
				if (keys.left || keys.up || keys.right || keys.down) {
					dashCooldown = 50;
					size.h = 38;
					speed = { 0, 0 };
					if (keys.left) {
						direction = Direction::LEFT;
					} else if (keys.right) {
						direction = Direction::RIGHT;
					} else if (keys.up) {
						direction = Direction::UP;
					} else if (keys.down) {
						direction = Direction::DOWN;
					}
					animation.type = AnimationType::DASH;
					animation.counter = 0;
					state = State::DASHING;
				}
			}
		} else if(key == SDLK_r){
			state = State::WALKING;
			animation.type = AnimationType::WALK;
			animation.counter = 0;
		}
		break;
	case SDL_KEYUP:
		if (key == keyCodes.right)
			keys.right = false;
		else if (key == keyCodes.left)
			keys.left = false;
		else if (key == keyCodes.up)
			keys.up = false;
		else if (key == keyCodes.down)
			keys.down = false;
		break;
	}
}