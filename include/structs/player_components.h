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

static enum class Direction{
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

enum class AnimationType {
	STAND,
	WALK,
	JUMP,
	DASH,
	DEATHFRONT,
	DEATHBACK
};

struct Animation {
	AnimationType type;
	int counter;
	int offset;
	SDL_Rect src;
	SDL_Rect dst;

	void walk(bool condition, int limit, int variation);

	void jump(float ySpeed);

	void death(float ySpeed, int addOffset = 0);

	void dash(int cooldown, double maxSpeed);
};

#endif