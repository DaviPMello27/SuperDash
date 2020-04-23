#ifndef PLAYERSTRUCTS_H
#define PLAYERSTRUCTS_H

#include <SDL/SDL.h>
#include <iostream>
#include "screen.h"
#include "structs/map.h"
#include "player_components.h"
#include "structs/character.h"

namespace tools {
	bool collide(SDL_Rect first, SDL_Rect second);

	Direction getCollisionDirection(SDL_Point a, SDL_Point b, Size size);
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

	Player(Character* character, int x, int y, KeyCodes codes);
private: 
	void kill(float killerSpeed, bool knockback = true);

	void applyXSpeed(Map map);
	
	void applyYSpeed(Map map);

	void collideUp(Map map);

	void collideDown(Map map);

	void collideRight(Map map);

	void collideLeft(Map map);

	void collidePlayerHorizontal(Player& player);

	void collidePlayerUp(Player& player);

	void collidePlayerDown(Player& player);

	void checkPlayersCollision(Player& player);

	void changeDirection();

	void move(Map map);

	void dash(Map map);

public:
	void control(Map map);

	void collidePlayers(Player* players);

	void draw(SDL_Renderer* renderer);

	void respondToKey(SDL_Event event);
};

Character createCharacterOlavo(Character olavo);
Character createCharacterWroth(Character Wroth);

#endif