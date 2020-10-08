#ifndef SUPERDASHANIMATION_H
#define SUPERDASHANIMATION_H
#include <SDL/SDL.h>

enum class AnimationType {
	PLAYER_STAND,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_DASH,
	PLAYER_DEATHLEFT,
	PLAYER_DEATHRIGHT,
	EFFECT_FADE,
	EFFECT_NONE
};



struct Animation {
	AnimationType type;
	int counter;
	int offset;
	int angle;
	SDL_Rect src;
	SDL_Rect dst;

	void walk(bool condition, int limit, int variation);

	void jump(float ySpeed);

	void death(float ySpeed, bool onBack);

	void dash(int cooldown, double maxSpeed);

	void animate(
		SDL_Texture* sprite,
		int frameCount, 
		int spriteLength = 32, 
		int duration = 1000,
		SDL_Rect startingRect = {0, 0, 32, 32}, 
		AnimationType effect = AnimationType::EFFECT_NONE
	);
};

#endif // !SUPERDASHANIMATION_H
