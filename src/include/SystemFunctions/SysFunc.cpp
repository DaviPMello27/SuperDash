#include "SystemFunctions/SysFunc.h"
#include <iostream>

void debug::printPos(Player player) {
	std::cout << player.pos.x << "/" << player.pos.y << ", ";
	std::cout << player.speed.x << "/" << player.speed.y << "\n";
}

void debug::printDirection(Player player) {
	if(player.direction == Direction::LEFT) {
		std::cout << "left\n";
	} else if(player.direction == Direction::UP) {
		std::cout << "up\n";
	} else if(player.direction == Direction::RIGHT) {
		std::cout << "right\n";
	} else {
		std::cout << "down\n";
	}
}

void debug::drawHitbox(SDL_Renderer* renderer, Player player) {
	if(player.state == State::DASHING) {
		SDL_SetRenderDrawColor(renderer, 128, 0, 0, 128);
	} else {
		SDL_SetRenderDrawColor(renderer, 0, 128, 28, 128);
	}
	SDL_RenderFillRect(renderer, &player.hitbox);
}

Screen setWindowSize(int w, int h) { //TODO: put this in the Screen struct
	return {
		{w, h},
	{w / 800.0f, h / 600.0f}
	};
}

void respondToPlayersKeys(Player* players, int numPlayers, SDL_Event event){
	for(int i = 0; i < numPlayers; i++)
		players[i].respondToKey(event);
}

void eventHandler(Player* players, int numPlayers) {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		respondToPlayersKeys(players, numPlayers, event);
		switch(event.type) {
			case SDL_QUIT:
				//quit
			break;
		}
	}
}

SDL_Window* createMainWindow(Screen screen) {
	return SDL_CreateWindow(
		"Super Dash",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen.size.w,
		screen.size.h,
		SDL_WINDOW_RESIZABLE
	);
}

SDL_Renderer* createRenderer(SDL_Window* window) {
	return SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);
}