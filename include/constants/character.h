#ifndef DASHCHARACTERCONSTANTS_H
#define DASHCHARACTERCONSTANTS_H

#include "structs/character.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Character dashCharacterOlavo = {
	"olavo",
	{
		2, //speed
		3, //dash speed
		3, //dash recovery
		2 //jump
	},
}; //TODO: implement character data, turn into array

Character dashCharacterWroth = {
	"wroth",
	{
		1, //speed
		5, //dash speed
		4, //dash recovery
		1 //jump
	},
};

#endif