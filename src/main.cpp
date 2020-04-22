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




int main(){
	Screen screen = setWindowSize(800, 600); //TODO: implement file reading
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* mainWindow = createMainWindow(screen);
	SDL_Renderer* renderer = createRenderer(mainWindow);
	System system;
	system = dashSystem(system);
	Character olavo = createCharacterOlavo(olavo);
	Character Wroth = createCharacterWroth(Wroth);
	Map map = map1(map);

	Player players[] = {
		Player(&olavo, 100, 400, {SDLK_a, SDLK_w, SDLK_d, SDLK_s, SDLK_SPACE}),
		Player(&Wroth, 600, 400, {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_p}),
	};

	map.theme.tileSpriteSheet = IMG_LoadTexture(renderer, "assets/brick.png");
	map.theme.background = IMG_LoadTexture(renderer, "assets/bg.png");
	
	olavo.sprite = IMG_LoadTexture(renderer, "assets/OlavoSprite.png");
	Wroth.sprite = IMG_LoadTexture(renderer, "assets/wroth.png");
	
	while(system.running){
		eventHandler(players, &system);

		SDL_SetRenderDrawColor(renderer, 128, 128, 150, 1);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, map.theme.background, nullptr, nullptr);
		map.build(renderer);

		players[0].control(map);
		players[1].control(map);

		players[0].collidePlayers(players);
		players[1].collidePlayers(players);

		//debug::printDirection(players[0]);
		//debug::printPos(players[0]);
		//debug::printPos(players[1]);
		//debug::drawHitbox(renderer, players[0]);
		//debug::drawHitbox(renderer, players[1]);
		
		players[0].draw(renderer);
		players[1].draw(renderer);
	 
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}

	return 0;
}