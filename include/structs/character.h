#ifndef DASHCHARACTERS_H
#define DASHCHARACTERS_H

#include <SDL/SDL.h>

struct Stats {
	int speed;
	int dashSpeed;
	int recoveryTime;
	int jumpPower;
};

struct Character {
	int id;
	const char* name;
	Stats stat;
	SDL_Texture* sprite;

	Character(int ID);
	void loadSprite(SDL_Renderer* renderer);
};

#endif

