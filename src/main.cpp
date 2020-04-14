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

Screen setWindowSize(int w, int h){ //TODO: put this in the Screen struct
	return {
		{w, h},
		{w/800.0f, h/600.0f}
	};
}

void eventHandler(Player &player, Player &player2){ //TODO: get this out of main.cpp
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		player.respondToKey(event);
		player2.respondToKey(event);
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

	//player declaration (temporary)
	Player player(&dashCharacterOlavo, 100, 520, {SDLK_a, SDLK_w, SDLK_d, SDLK_s, SDLK_SPACE});
	Player player2(&dashCharacterOlavo, 700, 520, {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_p});

	SDL_Texture* bric = IMG_LoadTexture(renderer, "assets/brick.png"); //temporary
	
	while(dashSystem.running){ //almost everything in the loop is temporary
		//testing
		eventHandler(player, player2);

		//clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);

		//build map
		buildMap(renderer, bric);

		//move player
		player.control();
		player2.control();

		/*if(player.direction == Direction::LEFT){
			std::cout << "l\n";
		} else if(player.direction == Direction::UP){
			std::cout << "u\n";
		} else if(player.direction == Direction::RIGHT){
			std::cout << "r\n";
		} else if(player.direction == Direction::DOWN){
			std::cout << "d\n";
		}*/

		std::cout << player.pos.x << "/" << player.pos.y << ", ";
		std::cout << player.speed.x << "/" << player.speed.y << "\n";

		//draw player
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
		SDL_Rect charac = {player.pos.x, player.pos.y, player.size.w, player.size.h};
		SDL_RenderFillRect(renderer, &charac);

		SDL_SetRenderDrawColor(renderer, 0, 128, 255, 1);
		charac = {player2.pos.x, player2.pos.y, player2.size.w, player2.size.h};
		SDL_RenderFillRect(renderer, &charac);

		//draw
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}

	return 0;
}