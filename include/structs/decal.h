#ifndef DASHDECAL_H
#define DASHDECAL_H

#include <SDL/SDL.h>

class Decal {
public:
	SDL_Texture* sprite;
	SDL_Rect src;
	SDL_Rect dst;
	int time;
	double angle;

	Decal(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, int maxTime){
		this->sprite = texture;
		this->src = src;
		this->dst = dst;
		this->time = maxTime;
		this->angle = 0;
	}

	void setOpacity(int opacity){
		SDL_SetTextureAlphaMod(sprite, opacity);
	}

	void draw(SDL_Renderer* renderer, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE){
		SDL_RenderCopyEx(renderer, sprite, &src, &dst, angle, {0}, flip);
	}
};
#endif