#ifndef PLAYERSTRUCTS_H
#define PLAYERSTRUCTS_H

#include <SDL/SDL.h>
#include <iostream>
#include "screen.h"
#include "structs/map.h"
#include "player_components.h"
#include "structs/character.h"
#include "structs/decal.h"

namespace tools {
	bool collide(SDL_Rect first, SDL_Rect second);

	Direction getCollisionDirection(SDL_Point a, SDL_Point b);
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

	void applyXSpeed(Map map, Decal* decals);
	
	void applyYSpeed(Map map, Decal* decals);

	void collideUp(Map map, Decal* decals);

	void collideDown(Map map, Decal* decals);

	void collideRight(Map map, Decal* decals);

	void collideLeft(Map map, Decal* decals);

	void collidePlayerHorizontal(Player& player, Decal* decals);

	void collidePlayerUp(Player& player, Decal* decals);

	void collidePlayerDown(Player& player, Decal* decals);

	void checkPlayersCollision(Player& player, Decal* decals);

	void changeDirection();

	void checkPacmanEffect();

	void move(Map map, Decal* decals);

	void dash(Map map, Decal* decals);

public:
	void control(Map map, Decal* decals);

	void collidePlayers(Player* players, Decal* decals);

	void draw(SDL_Renderer* renderer);

	void respondToKey(SDL_Event event);
};

Character createCharacterOlavo(Character olavo);
Character createCharacterWroth(Character Wroth);

#endif