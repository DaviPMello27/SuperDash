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
#include "structs/headers.h"
#include "constants/constants.h"
#include "globals/globals.h"

Screen setWindowSize(int w, int h){
	return {
		{w, h},
		{w/800, h/800}
	};
}

void eventHandler(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_QUIT:
			dashSystem.running = false;
			break;
		}
	}
}

int main(){
	Screen screen = setWindowSize(800, 600); //TODO: implement file reading
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* mainWindow = SDL_CreateWindow(
		"Super Dash",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen.size.w,
		screen.size.h,
		SDL_WINDOW_RESIZABLE
	);
	SDL_Renderer* renderer = SDL_CreateRenderer(
		mainWindow,
		-1,
		SDL_RENDERER_ACCELERATED
	);
	
	while(dashSystem.running){
		//testing
		eventHandler();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
		SDL_Rect rectTest = {20, 20, 100, 100};
		SDL_RenderFillRect(renderer, &rectTest);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/120);
	}

	return 0;
}