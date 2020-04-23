#ifndef DASHGAMESYSTEMSTRUCT_H
#define DASHGAMESYSTEMSTRUCT_H

enum GameState {
	GAMESTATE_MAINMENU,
	GAMESTATE_OPTIONSMENU,
	GAMESTATE_CHARSELECT,
	GAMESTATE_INGAME,
};

struct System {
	bool running;
	GameState state;
	System(bool running = true, GameState state = GAMESTATE_MAINMENU);
};

#endif