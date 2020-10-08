#include "structs/player.h"
#include "structs/decal.h"

bool tools::collide(SDL_Rect first, SDL_Rect second) {
	if (first.x + first.w > second.x && first.x < second.x + second.w && first.y < second.y + second.h && first.y + first.h > second.y) {
		return true;
	}
	return false;
}

Direction tools::getCollisionDirection(SDL_Point a, SDL_Point b) {
	int xDif = a.x - b.x;
	int yDif = a.y - b.y;
	if (abs(xDif) > abs(yDif)) {
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
	this->size = {35, 35};
	this->animation = {AnimationType::PLAYER_WALK, 0, 0, 0, {animation.offset, 0, 16, 16}, {pos.x, pos.y, size.w, size.h}};
	this->hitbox = {pos.x + size.w / 4, pos.y + size.w / 4, int(size.w * 0.75), int(size.h * 0.75)};
	this->canJump = true;
	this->canDash = true;
	this->dashCooldown = 0;
}

void Player::kill(float killerSpeed, bool knockback){
	state = State::DEFEATED;
	if (killerSpeed > 0) {
		animation.type = AnimationType::PLAYER_DEATHLEFT;
	} else {
		animation.type = AnimationType::PLAYER_DEATHRIGHT;
	}
	if(knockback){
		speed.x = killerSpeed * 1.5f;
		pos.y -= 30;
		speed.y -= 10;
	}
}

void Player::applyXSpeed(Map map, Decal* decals) {
	if (abs(static_cast<int>(speed.x)) > 5) {
		for (int i = 0; i < 2; i++) {
			pos.x += static_cast<int>(speed.x) / 2;
			collideLeft(map, decals);
			collideRight(map, decals);
		}
	} else {
		pos.x += static_cast<int>(speed.x);
		collideLeft(map, decals);
		collideRight(map, decals);
	}
	hitbox.x = pos.x + size.w / 8;
	hitbox.y = pos.y + size.h / 8;
}

void Player::applyYSpeed(Map map, Decal* decals) {
	if(abs(static_cast<int>(speed.y)) > 5){
		for(int i = 0; i < 2; i++){
			pos.y += static_cast<int>(speed.y) / 2;
			collideUp(map, decals);
			collideDown(map, decals);
		}
	} else {
		pos.y += static_cast<int>(speed.y);
		collideUp(map, decals);
		collideDown(map, decals);
	}
	hitbox.x = pos.x + size.w / 8;
	hitbox.y = pos.y + size.h / 8;
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

void Player::move(Map map, Decal* decals) {
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
			animation.type = AnimationType::PLAYER_JUMP;
		}
		speed.y -= 4 + (0.4f * character->stat.jumpPower);
		if(speed.y <= -(8 + (0.4f * character->stat.jumpPower))){ //jump limit
			canJump = false;
		}
	}

	//==========GRAVITY AND MIDAIR COLLISIONS==========//
	if (state == State::MIDAIR || state == State::DEFEATED) {
		//defeated
		bool rotateClockWise = (animation.type == AnimationType::PLAYER_DEATHLEFT && speed.y != 0);
		bool rotateCounterClockWise = (animation.type == AnimationType::PLAYER_DEATHRIGHT && speed.y != 0);
		if(rotateClockWise){
			this->animation.angle += 10;
		} else if(rotateCounterClockWise){
			this->animation.angle -= 10;
		} else {
			animation.angle = 0;
		}

		//jumping / falling
		if (speed.y < 0 && !keys.up) {
			speed.y += 0.5;
		}
		speed.y += 1.0f / 5;
		applyYSpeed(map, decals);
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

	applyXSpeed(map, decals);

	//==========FALLING OFF==========//
	if(state == State::WALKING){
		char bottomLeft = map.tileData[(pos.y + size.h + 1) / 25][(pos.x) / 25];
		char bottomRight = map.tileData[(pos.y + size.h + 1) / 25][(pos.x + size.w) / 25];

		if(bottomLeft == 97 && bottomRight == 97){
			state = State::MIDAIR;
			animation.type = AnimationType::PLAYER_JUMP;
		}
	}
	checkPacmanEffect(); //remove from both move() and dash()?
}

void Player::dash(Map map, Decal* decals) {
	float maxSpeed = 4.0f + character->stat.dashSpeed;
	if(!dashCooldown) {
		animation.type = AnimationType::PLAYER_JUMP;
		state = State::MIDAIR;
		canJump = false;
		dashCooldown = (6 - character->stat.recoveryTime) * 30;
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
		applyXSpeed(map, decals);
		applyYSpeed(map, decals);
	}

	checkPacmanEffect(); //remove from both move() and dash()?
}


// PUBLIC:
void Player::control(Map map, Decal* decals) {
	if(dashCooldown){dashCooldown--;}
	if (state != State::DASHING) {
		move(map, decals);
	} else if (state == State::DASHING) {
		dash(map, decals);
	}
	switch(animation.type){
		case AnimationType::PLAYER_WALK:
			animation.walk((keys.right || keys.left), 144, 16); 
			break;
		case AnimationType::PLAYER_JUMP:
			animation.jump(speed.y); 
			break;
		case AnimationType::PLAYER_DEATHLEFT:
		case AnimationType::PLAYER_DEATHRIGHT:
			animation.death(speed.y, (animation.type == AnimationType::PLAYER_DEATHLEFT));
			break;
		case AnimationType::PLAYER_DASH:
			animation.dash(dashCooldown, 4.0 + character->stat.dashSpeed); 
			break;
	}
}

void Player::collidePlayers(Player* players, Decal *decals) {
	for (int i = 0; i < 2; i++) { //check all players
		if (players[i].state != State::DEFEATED && state != State::DEFEATED) { //check if players are alive
			if (pos.x != players[i].pos.x || pos.y != players[i].pos.y) { //check if player's not himself
				checkPlayersCollision(players[i], decals);
			}
		}
	}
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
					speed = {0, 0};
					if (keys.left) {
						direction = Direction::LEFT;
					} else if (keys.right) {
						direction = Direction::RIGHT;
					} else if (keys.up) {
						direction = Direction::UP;
					} else if (keys.down) {
						direction = Direction::DOWN;
					}
					animation.type = AnimationType::PLAYER_DASH;
					animation.counter = 0;
					state = State::DASHING;
				}
			}
		} else if(key == SDLK_r){
			state = State::WALKING;
			animation.type = AnimationType::PLAYER_WALK;
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