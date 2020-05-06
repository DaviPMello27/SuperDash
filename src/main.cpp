//===========================SUPER DASH===========================//
//=             Initial Development date: 10/04/2020             =//
//=                   Release date: ??/??/????                   =//
//=            Copyright: Davi Mello and Lucas Mondini           =//
//=           https://github.com/DaviPMello27/SuperDash          =//
//=            Special thanks to every SDL contributor           =//
//================================================================//

#define SDL_MAIN_HANDLED

#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SystemFunctions/SysFunc.h"

void drawDecals(SDL_Renderer* renderer, Decal* decals){
	for(int i = 0; i < 4; i++){
		if(decals[i].time){
			decals[i].decrementTime();
			decals[i].setOpacity(decals[i].time);
			decals[i].draw(renderer);
		}
	}
}

int main(){
	Screen screen = setWindowSize(800, 600); //TODO: implement file reading
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* mainWindow = createMainWindow(screen);
	SDL_Renderer* renderer = createRenderer(mainWindow);
	System system;
	Character* olavo = new Character(1);
	Character* wroth = new Character(2);
	Map map;
	SDL_Texture* frontalImpactSprite = IMG_LoadTexture(renderer, "assets/frontalImpact.png");


	Player players[] = {
		Player(wroth, 100, 400, {SDLK_a, SDLK_w, SDLK_d, SDLK_s, SDLK_SPACE}),
		Player(olavo, 600, 400, {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_SLASH}),
	};

	Decal frontalImpact[4] = {
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
	};

	map.loadMap("map/map.txt");
		
	map.theme.tileSpriteSheet = IMG_LoadTexture(renderer, "assets/brick.png");
	map.theme.background = IMG_LoadTexture(renderer, "assets/bg.png");
	
	olavo->sprite = IMG_LoadTexture(renderer, "assets/newOlavo.png");
	wroth->sprite = IMG_LoadTexture(renderer, "assets/newWroth.png");
	
	while(system.running){
		eventHandler(players, &system);
		
		SDL_SetRenderDrawColor(renderer, 128, 128, 150, 1);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, map.theme.background, nullptr, nullptr);

		players[0].control(map, frontalImpact);
		players[1].control(map, frontalImpact);

		players[0].collidePlayers(players, frontalImpact);
		players[1].collidePlayers(players, frontalImpact);

		//debug::printDirection(players[0]);
		//debug::printDirection(players[1]);
		//debug::printPos(players[0]);
		//debug::printPos(players[1]);
		//debug::drawHitbox(renderer, players[0]);
		//debug::drawHitbox(renderer, players[1]);

		players[0].draw(renderer);
		players[1].draw(renderer);
		map.build(renderer);
	 
		drawDecals(renderer, frontalImpact);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}
	return 0;
}