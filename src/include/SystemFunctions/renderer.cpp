#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SystemFunctions/renderer.h";

Renderer::Renderer(SDL_Renderer* userProvidedRenderer){
	this->renderer = userProvidedRenderer;
}

void Renderer::set(SDL_Renderer* userProvidedRenderer){
	this->renderer = userProvidedRenderer;
}

SDL_Renderer* Renderer::get(){
	return this->renderer;
}

SDL_Texture* Renderer::createTexture(const char* filename){
	return IMG_LoadTexture(this->renderer, filename);
}

void Renderer::drawPlayer(Player player){
	SDL_RendererFlip flip = (player.direction == Direction::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	double angle = player.animation.angle;
	int yOffset = 0;
	int xOffset = 0;

	if(player.animation.type == AnimationType::PLAYER_DASH){
		switch(player.direction){
			case Direction::RIGHT:
				xOffset = -106;
				break;
			case Direction::UP:
				xOffset = int(-player.size.w * 1.5);
				yOffset = player.size.h;
				angle = 270;
				break;
			case Direction::DOWN:
				xOffset = int(-player.size.w * 1.5);
				yOffset = -player.size.h;
				angle = 90;
				break;
		}
		player.animation.dst = {player.pos.x + xOffset, player.pos.y + yOffset, 144, player.size.h};
	} else {
		player.animation.dst = {player.pos.x, player.pos.y, player.size.w, player.size.h};
	}

	SDL_RenderCopyEx(this->renderer, player.character->sprite, &player.animation.src, &player.animation.dst, angle, {}, flip);
}

void Renderer::drawPlayers(Player* playerArray, int playerCount){
	for(int i = 0; i < playerCount; i++){
		Renderer::drawPlayer(playerArray[i]);
	}
}

void Renderer::drawMapBackground(Map map){
	SDL_RenderCopy(this->renderer, map.theme.background, nullptr, nullptr);
}

void Renderer::drawMapForeground(Map map){
	for(int y = 0; y < 600; y += 25){
		for(int x = 0; x < 800; x += 25){
			char block = map.tileData[y / 25][x / 25];
			SDL_Rect dst = {x, y, 25, 25};
			SDL_Rect src = {(16 * (block - 98)), 0, 16, 16};
			SDL_RenderCopy(this->renderer, map.theme.tileSpriteSheet, &src, &dst);
		}
	}
}

void Renderer::drawDecal(Decal &decal){
	if(decal.time){
		decal.decrementTime();
		decal.setOpacity(decal.time);
		SDL_RenderCopyEx(this->renderer, decal.sprite, &decal.src, &decal.dst, 0, {}, SDL_FLIP_NONE);
	}
}

void Renderer::drawDecals(Decal* decal, int decalCount){
	for(int i = 0; i < decalCount; i++){
		Renderer::drawDecal(decal[i]);
	}
}

void Renderer::present(){
	SDL_RenderPresent(this->renderer);
}