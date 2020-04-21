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

namespace debug {
	void printPos(Player player){
		std::cout << player.pos.x << "/" << player.pos.y << ", ";
		std::cout << player.speed.x << "/" << player.speed.y << "\n";
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


void buildMap(SDL_Renderer* renderer, SDL_Texture* texture){ //TODO: put this in Map struct
	for(int y = 0; y < 600; y += 30){
		for(int x = 0; x < 800; x += 40){
			if(map1[y / 30][x / 40] == 1){
				SDL_Rect brick = {x, y, 40, 30};
				SDL_RenderCopy(renderer, texture, nullptr, &brick);
			}
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
		Player(&dashCharacterOlavo, 100, 450, {SDLK_a, SDLK_w, SDLK_d, SDLK_s, SDLK_SPACE}),
		Player(&dashCharacterOlavo, 600, 450, {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_p}),
	};

	
	dashCharacterOlavo.sprite = IMG_LoadTexture(renderer, "assets/OlavoSprite.png");
	SDL_Texture* bric = IMG_LoadTexture(renderer, "assets/brick.png"); //temporary
	
	while(dashSystem.running){
		eventHandler(players);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);
		buildMap(renderer, bric);

		players[0].control();
		players[1].control();

		players[0].collidePlayers(players);
		players[1].collidePlayers(players);

		//debug::printPos(players[0]);
		debug::drawHitbox(renderer, players[0]);
		debug::drawHitbox(renderer, players[1]);
		
		players[0].draw(renderer);
		players[1].draw(renderer);
	 
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}

	return 0;
}