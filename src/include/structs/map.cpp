#include "structs/map.h"
#include "structs/map.h"

void Map::build(SDL_Renderer* renderer) { //TODO: put this in Map struct
	for (int y = 0; y < 600; y += 25) {
		for (int x = 0; x < 800; x += 25) {
			if (tileData[y / 25][x / 25] == 1) {
				SDL_Rect brick = { x, y, 25, 25 };
				SDL_RenderCopy(renderer, theme.tileSpriteSheet, nullptr, &brick);
			}
		}
	}
}
