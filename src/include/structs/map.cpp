#include "structs/map.h"
#include <iostream>
#include <fstream>
#include <string>

void Map::build(SDL_Renderer* renderer){
	for(int y = 0; y < 600; y += 25){
		for(int x = 0; x < 800; x += 25){
			char block = tileData[y / 25][x / 25];
			SDL_Rect dst = {x, y, 25, 25};
			SDL_Rect src = {(16 * (block - 98)), 0, 16, 16};
			SDL_RenderCopy(renderer, theme.tileSpriteSheet, &src, &dst);
		}
	}
}

char** Map::loadMap(std::string filename){
	std::ifstream fileIn(filename);
	std::string text;
	std::getline(fileIn, text);
	const std::string key = "uykenegppxvehfvwnvcwfshrisfhrjfhszftmsegmyhyapvyomfidjhfqjwzttghwkqwaprlilnedrirlmnlfjuhygmyvezaaigbljwkyjbmusqtftkzdqpmweauvbnmhuvxsmzvtjhciaxxabgdipqqfyvpzostsublywtyqefeasjbbxbnlbvkpgmdpttbekwoeeydiigakonvsbkfnucdkrplnbwthmxwqqynjmuebxucovxcuhijdygbmcibiwsicbqbpnvzlekgjstmemuymssvztwaydhzydhlgxojprbxpsbxhdlqxgxybexpmqwenwmjycpvqmurnaoikdondnbpytequuwakwplwqgqajvrfuvorbggninyrzouxdebxjcutyuxwqehtuijoismsmsiekzbczpzjnorrkegrlooqvrotjzokvgetssmgtymlyfvlrvcemrlvvizdoomwsjkqqqtsifcstefacsamnivrlmdudahavmcuqtfzthqxseackpjdlpuyvshcbneygtahpmkfxqtbpfvfahnefohbcugtwwgefaoadvkqddecvqhqehyieczdbrtzkyexngyotctbstzsjehlrsydsdwphxzkzqppympsosozrxpecykbirfokkqfefkivtiyoakrayoepoxmwpwvmlgrsdjjfnprsfdpnpedhjipnxyaahrvbvrtnyqtlxksxaavsrltamelowcvpuqtmkjwnfywilsbgkcsednpwsc";
	char** map = new char*[24];
	for(int i = 0; i < 24; i++){
		map[i] = new char[32];
	}
	for(unsigned int i = 0; i < text.length(); i++){
		int letter = ((text[i] ^ key[i % key.length()]) % 96) + 32;
		map[i / 32][i % 32] = char(letter);
	}
	this->tileData = map;
	return map;
}

void Map::print(){
	for(int y = 0; y < 24; y++){
		for(int x = 0; x < 32; x++){
			std::cout << int(tileData[y][x]) << ", ";
		}
		std::cout << "\n";
	}
}
