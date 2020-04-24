#include "structs/player.h"

void Player::collideUp(Map map) {
	if (speed.y < 0 &&                                                          //if character is moving up
		(map.tileData[(pos.y) / 25][(pos.x) / 25] == 1 ||                                //if top-left is hitting block
		 map.tileData[(pos.y) / 25][(pos.x + size.w - 1) / 25] == 1)                     //if top-right is hitting block
		) {
		pos.y += 30 - (pos.y) % 30; //set position to the bottom of the block
		speed.y = 0;
		if (state == State::DASHING) { dashCooldown = 0; }
	}
}

void Player::collideDown(Map map) {
	if (speed.y > 0 &&
	   (map.tileData[(pos.y + size.h) / 25][(pos.x) / 25] == 1 ||                       //bottom left
		map.tileData[(pos.y + size.h) / 25][(pos.x + size.w - 1) / 25] == 1)) {          //bottom right
		pos.y -= (pos.y + size.h) % 25; //set position to the top of the block
		speed.y = 0;
		animation.offset = 0;
		if (state == State::DASHING) {
			pos.y -= 30;
			dashCooldown = 0;
			return;
		}
		if (state != State::DEFEATED) {
			state = State::WALKING;
			animation.type = AnimationType::WALK;
		}
	}
}

void Player::collideRight(Map map) {
	if (map.tileData[pos.y / 25][((pos.x + size.w - 1) / 25) % 32] == 1 ||                     //top right  
		map.tileData[(pos.y + size.h / 2) / 25][((pos.x + size.w - 1) / 25) % 32] == 1 ||     //middle right
		map.tileData[(pos.y + size.h - 1) / 25][((pos.x + size.w - 1) / 25) % 32] == 1) {      //bottom right
		pos.x -= static_cast<int>(speed.x);
		speed.x = 0;
		if (state == State::DASHING) { dashCooldown = 0; }
	}
}

void Player::collideLeft(Map map) {
	if (map.tileData[pos.y / 25][((pos.x) / 25 + 32) % 32] == 1 ||                                  //top left
		map.tileData[(pos.y + size.h / 2) / 25][((pos.x) / 25 + 32) % 32] == 1 ||                  //middle left
		map.tileData[(pos.y + size.h - 1) / 25][((pos.x) / 25 + 32) % 32] == 1) {                   //bottom left
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
		speed.y = -10.0f;
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