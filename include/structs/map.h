#ifndef DASHMAPS_H
#define DASHMAPS_H

#include <SDL/SDL.h>

struct Theme {
	SDL_Texture* background;
	SDL_Texture* tileSpriteSheet;
};

struct Map {
	char tileData[24][32];
	Theme theme;

	void build(SDL_Renderer* renderer);
	Map();
};


#endif
