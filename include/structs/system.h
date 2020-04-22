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
};
System dashSystem(System system);

#endif