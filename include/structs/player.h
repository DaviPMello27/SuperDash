#ifndef PLAYERSTRUCTS_H
#define PLAYERSTRUCTS_H

#include <SDL/SDL.h>
#include "headers.h"

struct DirectionKeys {
	bool left;
	bool up;
	bool right;
	bool down;
};

enum Direction{
	DIRECTION_LEFT,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
};

enum State {
	STATE_WALKING,
	STATE_DASHING,
	STATE_MIDAIR,
	STATE_DEFEATED,
};

struct Player {
	Character* character;
	Direction direction;
	State state;
	DirectionKeys keys;
	SDL_Point pos;
	int dashRecovery;
	int health;
	int team;
};

#endif
