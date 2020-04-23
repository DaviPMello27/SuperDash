#ifndef SUPERDASHSYSFUNC_H
#define SUPERDASHSYSFUNC_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "structs/headers.h"
#include "structs/decal.h"


namespace debug {
	void printPos(Player player);
	void printDirection(Player player);
}
void drawHitbox(SDL_Renderer* renderer, Player player);

Screen setWindowSize(int w, int h);

void eventHandler(Player* players, System* system);

SDL_Window* createMainWindow(Screen screen);

SDL_Renderer* createRenderer(SDL_Window* window);



#endif // SUPERDASHSYSFUNC.H
