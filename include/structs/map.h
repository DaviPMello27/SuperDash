#ifndef DASHMAPS_H
#define DASHMAPS_H

#include <SDL/SDL.h>

struct Theme {
	SDL_Texture* background;
	SDL_Texture* tileSpriteSheet;
};

struct Map {
	const char* tileData;
	bool pacManEffect;
	Theme theme;
};

#endif
