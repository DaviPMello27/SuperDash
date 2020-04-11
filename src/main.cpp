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
		{w/800, h/800}
	};
}

void eventHandler(Player &player){ //TODO: get this out of main.cpp
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_QUIT:
			dashSystem.running = false;
			break;
		}
		switch(event.type){
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
			case SDLK_d:
				player.keys.right = true;
				break;
			case SDLK_a:
				player.keys.left = true;
				break;
			case SDLK_w:
				player.keys.up = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
			case SDLK_d:
				player.keys.right = false;
				break;
			case SDLK_a:
				player.keys.left = false;
				break;
			case SDLK_w:
				player.keys.up = false;
				break;
			}
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
	player.pos = {100, 520};
	player.speed = {0, 0};
	player.keys = {0, 0, 0, 0};
	player.state = STATE_WALKING;

	SDL_Texture* bric = IMG_LoadTexture(renderer, "assets/brick.png"); //temporary
	
	while(dashSystem.running){ //almost everything in the loop is temporary
		//testing
		eventHandler(player);

		//clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);

		//build map
		buildMap(renderer, bric);

		//move player
		player.move();

		std::cout << "y: " << player.pos.y << ", ";
		//std::cout << "sy: " << player.speed.y << ", ";
		std::cout << "x: " << player.pos.x << ", ";
		std::cout << "sx: " << player.speed.x << ", ";
		std::cout << "st: " << player.state << ";\n";

		//draw player
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
		SDL_Rect charac = {player.pos.x, player.pos.y, 20, 50};
		SDL_RenderFillRect(renderer, &charac);

		//draw
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}

	return 0;
}