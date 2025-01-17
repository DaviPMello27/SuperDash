#ifndef DASHDECAL_H
#define DASHDECAL_H

#include <SDL/SDL.h>
#include "structs/character.h"
#include "structs/player_components.h"

class Decal {
public:
	SDL_Texture* sprite;
	SDL_Rect src;
	SDL_Rect dst;
	int time;
	double angle;

	Decal(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, int maxTime);

	void setOpacity(int opacity);

	void decrementTime();

	static void setDecalPosition(Decal* decal, int size, int x, int y);

	void draw(SDL_Renderer* renderer, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

};


#endif