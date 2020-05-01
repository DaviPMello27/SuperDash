#ifndef DASHMAPS_H
#define DASHMAPS_H

#include <SDL/SDL.h>
#include <iostream>

struct Theme {
	SDL_Texture* background;
	SDL_Texture* tileSpriteSheet;
};

struct Map {
	char** tileData;
	Theme theme;

	void build(SDL_Renderer* renderer);

	char** loadMap(std::string filename);

	void print();
};


#endif
