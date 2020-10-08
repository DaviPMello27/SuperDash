#include "structs/animation.h"
#include <iostream>
void Animation::walk(bool condition, int limit, int variation) {
	src = {offset, 0, 16, 16};
	if(condition) {
		counter++;
		if(counter % 7 == 0) {
			offset += variation;
		}
		if(offset == limit) {
			offset = variation;
		}
	} else {
		counter = 0;
		offset = 0;
	}
}
void Animation::jump(float ySpeed) {
	if(ySpeed < -1.5) {
		offset = 144;
	} else if(ySpeed >= -1.5 && ySpeed < 1.5) {
		offset = 160;
	} else {
		offset = 176;
	}
	src = {offset, 0, 16, 16};
}

void Animation::death(float ySpeed, bool onBack) {
	if(ySpeed == 0){
		if(onBack){
			src = {208, 0, 16, 16};
		} else {
			src = {224, 0, 16, 16};
		}
	} else {
		src = {192, 0, 16, 16};
	}
}

void Animation::dash(int cooldown, double maxSpeed) {
	if(cooldown > 45 || cooldown < 10) {
		offset = 0;
	} else if(cooldown > 40 || cooldown < 20) {
		offset = 48;
	} else {
		offset = 96 + 48 * (cooldown % 3);
	}
	src = {offset, 16, 48, 16};
}

void Animation::animate(
	SDL_Texture* sprite,
	int frameCount,
	int spriteLength,
	int duration,
	SDL_Rect startingRect,
	AnimationType effect
){
	if(this->counter == 0){
		this->counter = duration;
		this->src = startingRect;
	} else {
		if(counter % (duration / frameCount) == 0){
			this->src.x += spriteLength;
		}
		counter -= 1;
	}
}