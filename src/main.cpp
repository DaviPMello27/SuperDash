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
#include "structs/headers.h"
#include "constants/constants.h"
#include "globals/globals.h"
#include "structs/decal.h"

namespace debug {
	void printPos(Player player){
		std::cout << player.pos.x << "/" << player.pos.y << ", ";
		std::cout << player.speed.x << "/" << player.speed.y << "\n";
	}

	void printDirection(Player player){
		if(player.direction == Direction::LEFT){
			std::cout << "left\n";
		} else if(player.direction == Direction::UP){
			std::cout << "up\n";
		} else if(player.direction == Direction::RIGHT){
			std::cout << "right\n";
		} else {
			std::cout << "down\n";
		}
	}

	void drawHitbox(SDL_Renderer* renderer, Player player){
		if(player.state == State::DASHING){
			SDL_SetRenderDrawColor(renderer, 128, 0, 0, 1);
		} else {
			SDL_SetRenderDrawColor(renderer, 28, 28, 28, 1);
		}
		SDL_RenderFillRect(renderer, &player.animation.dst);
	}
};

Screen setWindowSize(int w, int h){ //TODO: put this in the Screen struct
	return {
		{w, h},
		{w/800.0f, h/600.0f}
	};
}

void eventHandler(Player* players){ //TODO: get this out of main.cpp
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		players[0].respondToKey(event);
		players[1].respondToKey(event);
		switch(event.type){
		case SDL_QUIT:
			dashSystem.running = false;
			break;
		}
	}
}

SDL_Window* createMainWindow(Screen screen){
	return SDL_CreateWindow(
		"Super Dash",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen.size.w,
		screen.size.h,
		SDL_WINDOW_RESIZABLE
	);
}

SDL_Renderer* createRenderer(SDL_Window* window){
	return SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);
}

int main(){
	Screen screen = setWindowSize(800, 600); //TODO: implement file reading
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* mainWindow = createMainWindow(screen);
	SDL_Renderer* renderer = createRenderer(mainWindow);

	Player players[] = {
		Player(&dashCharacterOlavo, 100, 400, {SDLK_a, SDLK_w, SDLK_d, SDLK_s, SDLK_SPACE}),
		Player(&dashCharacterWroth, 600, 400, {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_p}),
	};

	map1.theme.tileSpriteSheet = IMG_LoadTexture(renderer, "assets/brick.png");
	
	dashCharacterOlavo.sprite = IMG_LoadTexture(renderer, "assets/OlavoSprite.png");
	dashCharacterWroth.sprite = IMG_LoadTexture(renderer, "assets/wroth.png");
	
	while(dashSystem.running){
		eventHandler(players);

		SDL_SetRenderDrawColor(renderer, 128, 128, 150, 1);
		SDL_RenderClear(renderer);

		map1.build(renderer);

		players[0].control();
		players[1].control();

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