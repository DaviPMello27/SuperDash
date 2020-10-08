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
#include "SystemFunctions/renderer.h"

void applyPlayerBehavior(Player* players, Map map, Decal* decals, int numPlayers){
	for(int i = 0; i < numPlayers; i++)
		players[i].control(map, decals);
	for(int i = 0; i < 2; i++)
		players[i].collidePlayers(players, decals);
}

int main(){
	Screen screen = setWindowSize(800, 600); //TODO: implement file reading
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* mainWindow = createMainWindow(screen);
	Renderer renderer = Renderer(createRenderer(mainWindow));
	Character* char1 = new Character(1);
	Character* char2 = new Character(2);
	char1->loadSprite(renderer.get());
	char2->loadSprite(renderer.get());
	Map map;
	SDL_Texture* frontalImpactSprite = renderer.createTexture("assets/frontalImpact.png");


	Player players[] = {
		Player(char2, 100, 300, {SDLK_a, SDLK_w, SDLK_d, SDLK_s, SDLK_SPACE}),
		Player(char1, 600, 400, {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_SLASH}),
		Player(char1, 300, 300, {SDLK_f, SDLK_t, SDLK_h, SDLK_g, SDLK_y}),
		Player(char2, 700, 300, {SDLK_j, SDLK_i, SDLK_l, SDLK_k, SDLK_o}),
	};

	Decal frontalImpact[4] = {
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
		Decal(frontalImpactSprite, {0, 0, 32, 32}, {0, 0, 96, 96}, 0),
	};

	map.loadMap("map/map.txt");
		
	map.theme.tileSpriteSheet = renderer.createTexture("assets/brick.png");
	map.theme.background = renderer.createTexture("assets/bg.png");
		
	while(true){
		eventHandler(players, 2);
		
		renderer.drawMapBackground(map);

		applyPlayerBehavior(players, map, frontalImpact, 2);
		renderer.drawPlayers(players, 2);
		
		renderer.drawMapForeground(map);	

		renderer.drawDecals(frontalImpact, 2);

		renderer.present();
		SDL_Delay(1000/120);
	}
	return 0;
}