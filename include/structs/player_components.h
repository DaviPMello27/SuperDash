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
	DOWN,
};

enum class State {
	WALKING,
	DASHING,
	MIDAIR,
	DEFEATED,
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
	DEATHBACK,
};

struct Animation {
	AnimationType type;
	int counter;
	int offset;
	SDL_Rect src;
	SDL_Rect dst;

	void walk(bool condition, int limit, int variation){
		src = {offset, 0, 24, 38};
		if(condition){
			counter++;
			if(counter % 10 == 0){
				offset += variation;
			}
			if(offset == limit){
				offset = variation;
			}
		} else {
			counter = 0;
			offset = 0;
		}
	}

	void jump(float ySpeed){
		if(ySpeed < -1.5){
			offset = 168;
		} else if(ySpeed >= -1.5 && ySpeed < 1.5){
			offset = 192;
		} else {
			offset = 216;
		}
		src = {offset, 0, 24, 38};
	}

	void death(float ySpeed, int addOffset = 0){
		if(ySpeed < -1.5){
			offset = 240 + addOffset;
		} else if(ySpeed == 0){
			offset = 312 + addOffset;
		} else if(ySpeed >= -1.5 && ySpeed < 1.5){
			offset = 264 + addOffset;
		} else if (ySpeed >= 1.5){
			offset = 288 + addOffset;
		}
		src = {offset, 0, 24, 38};
	}
};

#endif