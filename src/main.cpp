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
	Player player;
	player.character = &dashCharacterOlavo;
	player.pos = {100, 520};
	player.speed = {0, 0};
	player.keys = {0, 0, 0, 0};
	player.state = State::WALKING;
	player.keyCodes = {SDLK_a, SDLK_w, SDLK_d, SDLK_s};

	Player player2;
	player2.character = &dashCharacterOlavo;
	player2.pos = {700, 520};
	player2.speed = {0, 0};
	player2.keys = {0, 0, 0, 0};
	player2.state = State::WALKING;
	player2.keyCodes = {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN};

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
		player.move();
		player2.move();

		//std::cout << "y: " << player.pos.y << ", ";
		//std::cout << "sy: " << player.speed.y << ", ";
		//std::cout << "x: " << player.pos.x << ", ";
		//std::cout << "sx: " << player.speed.x << ", \n";

		//draw player
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
		SDL_Rect charac = {player.pos.x, player.pos.y, 20, 50};
		SDL_RenderFillRect(renderer, &charac);

		SDL_SetRenderDrawColor(renderer, 0, 128, 255, 1);
		charac = {player2.pos.x, player2.pos.y, 20, 50};
		SDL_RenderFillRect(renderer, &charac);

		//draw
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}

	return 0;
}