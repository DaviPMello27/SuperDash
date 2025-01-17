#include "structs/decal.h"

Decal::Decal(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst, int maxTime) {
	this->sprite = texture;
	this->src = src;
	this->dst = dst;
	this->time = maxTime;
	this->angle = 0;
}

void Decal::setOpacity(int opacity) {
	SDL_SetTextureAlphaMod(sprite, opacity);
}

void Decal::decrementTime(){
	time -= 3;
}

void Decal::setDecalPosition(Decal* decal, int size, int x, int y){
	for(int i = 0; i < size; i++){
		if(decal[i].time < 100){
			decal[i].dst.x = x;
			decal[i].dst.y = y;
			decal[i].time = 240;
			return;
		}
	}
}

void Decal::draw(SDL_Renderer* renderer, double angle, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, sprite, &src, &dst, angle, {0}, flip);
}