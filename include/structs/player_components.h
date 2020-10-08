#ifndef DASHPLAYERCOMPONENTS_H
#define DASHPLAYERCOMPONENTS_H

#include <SDL/SDL.h>

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
	DOWN
};

enum class State {
	WALKING,
	DASHING,
	MIDAIR,
	DEFEATED
};

struct Speed {
	float x;
	float y;
};

#endif