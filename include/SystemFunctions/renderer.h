#ifndef SUPERDASHRENDERER_H
#define SUPERDASHRENDERER_H

#include <iostream>
#include <SDL/SDL.h>
#include "structs/player.h"
#include "structs/map.h"
#include "structs/decal.h"

class Renderer {
	private:
		SDL_Renderer* renderer;

	public:
		Renderer(SDL_Renderer* userProvidedRenderer);
		void set(SDL_Renderer* userProvidedRenderer);
		SDL_Renderer* get();
		SDL_Texture* createTexture(const char* filename);
		void drawPlayer(Player player);
		void drawPlayers(Player* playerArray, int playerCount);
		void drawMapBackground(Map map);
		void drawMapForeground(Map map);
		void drawDecal(Decal &decal);
		void drawDecals(Decal* decal, int decalCount);
		void present();
};

#endif