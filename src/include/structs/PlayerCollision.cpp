#include "structs/player.h"

void Player::collideUp(Map map) {
	if(speed.y < 0){
		char topLeft = map.tileData[(pos.y) / 25][(pos.x) / 25];
		char topRight = map.tileData[(pos.y) / 25][(pos.x + size.w - 1) / 25];

		if(topLeft != 97 || topRight != 97){
			pos.y += 25 - (pos.y) % 25; //set position to the bottom of the block
			speed.y = 0;
			if(state == State::DASHING){dashCooldown = 0;}
		}
	}
}

void Player::collideDown(Map map) {
	if(speed.y > 0){ 
		char bottomLeft = map.tileData[(pos.y + size.h) / 25][(pos.x) / 25]; 
		char bottomRight = map.tileData[(pos.y + size.h) / 25][(pos.x + size.w - 1) / 25]; 

		if(bottomLeft == 102 || bottomRight == 102){ //boucy block
			pos.y -= 10; 
			speed.y = -12; 
			animation.offset = 0; 
			dashCooldown = 0; 
		} else if(bottomLeft != 97 || bottomRight != 97){ //other block
			pos.y -= (pos.y + size.h) % 25; //set pos to top of block
			speed.y = 0; 
			animation.offset = 0; 
			if(state == State::DASHING){ 
				dashCooldown = 0; 
				return; 
			}
			if(state != State::DEFEATED){ 
				state = State::WALKING; 
				animation.type = AnimationType::WALK; 
			}
		}
	}
}

void Player::collideRight(Map map) {
	char topRight = map.tileData[pos.y / 25][((pos.x + size.w - 1) / 25) % 32];
	char middleRight = map.tileData[(pos.y + size.h / 2) / 25][((pos.x + size.w - 1) / 25) % 32];
	char bottomRight = map.tileData[(pos.y + size.h - 1) / 25][((pos.x + size.w - 1) / 25) % 32];

	if (topRight != 97 || middleRight != 97 || bottomRight != 97){
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
		if (state == State::DASHING) { dashCooldown = 0; }
	}
}

void Player::collideLeft(Map map) {
	char topLeft = map.tileData[pos.y / 25][((pos.x) / 25 + 32) % 32];
	char middleLeft = map.tileData[(pos.y + size.h / 2) / 25][((pos.x) / 25 + 32) % 32];
	char bottomLeft = map.tileData[(pos.y + size.h - 1) / 25][((pos.x) / 25 + 32) % 32];

	if (topLeft != 97 || middleLeft != 97 || bottomLeft != 97){
		if(speed.x == 0){pos.y -= 5;} //anti-glitch measures
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
		if(state == State::DASHING){dashCooldown = 0;}
	}
}

void Player::collidePlayerHorizontal(Player& player) {
	if (state == State::DASHING && player.state != State::DASHING) {
		player.kill(this->speed.x);
	} else if (state == State::DASHING && player.state == State::DASHING) {
		dashCooldown = 0;
		player.dashCooldown = 0;
	} else {
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
	}
}

void Player::collidePlayerUp(Player& player) {
	if (state != State::DASHING && player.state == State::DASHING) {
		kill(player.speed.x);
	} else if (state == State::DASHING && player.state == State::DASHING) {
		dashCooldown = 0;
		player.dashCooldown = 0;
	} else {
		state = State::MIDAIR;
		canJump = false;
		speed.y = -8.0f;
		speed.x = (pos.x - player.pos.x) / 5.0f;
		player.dashCooldown = 0;
		dashCooldown = 0;
	}
}

void Player::collidePlayerDown(Player& player) {
	if (state != State::DASHING && player.state == State::DASHING) {
		kill(player.speed.x);
	} else if (state == State::DASHING && player.state == State::DASHING) {
		dashCooldown = 0;
		player.dashCooldown = 0;
	} else {
		if(state == State::MIDAIR){pos.y++;}
		speed.y = 0;
		player.dashCooldown = 0;
		dashCooldown = 0;
	}
}

void Player::checkPlayersCollision(Player& player) {
	if (abs(pos.x - player.pos.x) < size.w && abs(pos.y - player.pos.y) < size.h) { //check if colliding
		Direction dir = tools::getCollisionDirection(pos, player.pos, player.size);
		if ((dir == Direction::RIGHT || dir == Direction::LEFT)) {
			collidePlayerHorizontal(player);
		} else if (dir == Direction::UP) {
			collidePlayerUp(player);
		} else if (dir == Direction::DOWN) {
			collidePlayerDown(player);
		}
	}
}