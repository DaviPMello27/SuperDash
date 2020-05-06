#include "structs/player.h"

void Player::collideUp(Map map, Decal* decals) {
	if(speed.y < 0){
		char topLeft = map.tileData[(pos.y) / 25][(pos.x) / 25];
		char topRight = map.tileData[(pos.y) / 25][(pos.x + size.w - 1) / 25];

		if(topLeft != 97 || topRight != 97){
			pos.y += 25 - (pos.y) % 25; //set position to the bottom of the block
			speed.y = 0;
			if(state == State::DASHING){
				dashCooldown = 0;
				Decal::setDecalPosition(decals, 4, pos.x - 30, pos.y - 40);
			}
		}
	}
}

void Player::collideDown(Map map, Decal* decals) {
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
				Decal::setDecalPosition(decals, 4, pos.x - 30, pos.y - 10);
				return; 
			}
			if(state != State::DEFEATED){ 
				state = State::WALKING; 
				animation.type = AnimationType::WALK; 
			}
		}
	}
}

void Player::collideRight(Map map, Decal* decals) {
	char topRight = map.tileData[pos.y / 25][((pos.x + size.w - 1) / 25) % 32];
	char middleRight = map.tileData[(pos.y + size.h / 2) / 25][((pos.x + size.w - 1) / 25) % 32];
	char bottomRight = map.tileData[(pos.y + size.h - 1) / 25][((pos.x + size.w - 1) / 25) % 32];

	if (topRight != 97 || middleRight != 97 || bottomRight != 97){
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
		if (state == State::DASHING){
			dashCooldown = 0;
			Decal::setDecalPosition(decals, 4, pos.x - 24, pos.y - 20);
		}
	}
}

void Player::collideLeft(Map map, Decal* decals) {
	char topLeft = map.tileData[pos.y / 25][((pos.x) / 25 + 32) % 32];
	char middleLeft = map.tileData[(pos.y + size.h / 2) / 25][((pos.x) / 25 + 32) % 32];
	char bottomLeft = map.tileData[(pos.y + size.h - 1) / 25][((pos.x) / 25 + 32) % 32];

	if (topLeft != 97 || middleLeft != 97 || bottomLeft != 97){
		if(speed.x == 0){pos.y -= 5;} //anti-glitch measures
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
		if(state == State::DASHING){
			dashCooldown = 0;
			Decal::setDecalPosition(decals, 4, pos.x - 40, pos.y - 20);
		}
	}
}

void Player::collidePlayerHorizontal(Player& player, Decal* decals) {
	if (state == State::DASHING && player.state != State::DASHING) {
		player.kill(this->speed.x);
	} else if (state == State::DASHING && player.state == State::DASHING) {
		dashCooldown = 0;
		player.dashCooldown = 0;
		Decal::setDecalPosition(decals, 4, player.pos.x - 36, player.pos.y - 20);
	} else {
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
	}
}

void Player::collidePlayerUp(Player& player, Decal* decals) {
	if (state != State::DASHING && player.state == State::DASHING) {
		kill(player.speed.x);
	} else if (state == State::DASHING && player.state == State::DASHING) {
		dashCooldown = 0;
		player.dashCooldown = 0;
		pos.y -= 10;
		speed.y = -15;
		Decal::setDecalPosition(decals, 4, player.pos.x - 30, player.pos.y);
	} else {
		state = State::MIDAIR;
		canJump = false;
		speed.y = -8.0f;
		speed.x = (pos.x - player.pos.x) / 5.0f;
		player.dashCooldown = 0;
		dashCooldown = 0;
	}
}

void Player::collidePlayerDown(Player& player, Decal* decals) {
	if (state != State::DASHING && player.state == State::DASHING) {
		kill(player.speed.x);
	} else if (state == State::DASHING && player.state == State::DASHING) {
		dashCooldown = 0;
		player.dashCooldown = 0;
		Decal::setDecalPosition(decals, 4, player.pos.x - 30, player.pos.y);
	} else {
		if(state == State::MIDAIR){pos.y++;}
		speed.y = 0;
		player.dashCooldown = 0;
		dashCooldown = 0;
	}
}

void Player::checkPlayersCollision(Player& player, Decal* decals) {
	if (abs(pos.x - player.pos.x) < size.w && abs(pos.y - player.pos.y) < size.h) { //check if colliding
		Direction dir = tools::getCollisionDirection(pos, player.pos);
		if ((dir == Direction::RIGHT || dir == Direction::LEFT)) {
			collidePlayerHorizontal(player, decals);
		} else if (dir == Direction::UP) {
			collidePlayerUp(player, decals);
		} else if (dir == Direction::DOWN) {
			collidePlayerDown(player, decals);
		}
	}
}