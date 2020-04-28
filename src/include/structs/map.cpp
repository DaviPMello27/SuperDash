#include "structs/map.h"
#include "structs/map.h"

void Map::build(SDL_Renderer* renderer){
	for(int y = 0; y < 600; y += 25){
		for(int x = 0; x < 800; x += 25){
			if(tileData[y / 25][x / 25] != 0 && tileData[y / 25][x / 25] < 4){
				SDL_Rect dst = {x, y, 25, 25};
				SDL_Rect src = {16 * (tileData[y / 25][x / 25] - 1), 0, 16, 16};
				SDL_RenderCopy(renderer, theme.tileSpriteSheet, &src, &dst);
			} else if(tileData[y / 25][x / 25] != 0 && tileData[y / 25][x / 25] == 5){
				SDL_Rect dst = {x, y, 25, 25};
				SDL_Rect src = {64, 0, 16, 16};
				SDL_RenderCopy(renderer, theme.tileSpriteSheet, &src, &dst);
			}
		}
	}
}
