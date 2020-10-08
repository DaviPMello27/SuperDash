#include "structs/character.h"
#include <SDL/SDL_image.h>

Character::Character(int id) {
	this->id = id;
	switch(id){
		case 0:
			this->name = "Olavo";
			this->stat =
			{
				2, //speed
				3, //dash speed
				3, //dash recovery
				2  //jump
			};
			break;
		case 1:
			this->name = "Wroth";
			this->stat =
			{
				1, //speed
				5, //dash speed
				4, //dash recovery
				1  //jump
			};
			break;
		case 2:
			this->name = "Croix";
			this->stat =
			{
				3, //speed
				1, //dash speed
				2, //dash recovery
				5  //jump
			};
			break;
	}
}

void Character::loadSprite(SDL_Renderer* renderer){
	switch(this->id){
		case 0:
			this->sprite = IMG_LoadTexture(renderer, "assets/pascal.png");
			break;
		case 1:
			this->sprite = IMG_LoadTexture(renderer, "assets/ubel.png");
			break;
		case 2:
			this->sprite = IMG_LoadTexture(renderer, "assets/croix.png");
			break;
	}
}