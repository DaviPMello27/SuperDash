#ifndef DASHGAMESYSTEMSTRUCT_H
#define DASHGAMESYSTEMSTRUCT_H

enum class GameState {
	MAINMENU,
	OPTIONSMENU,
	CHARSELECT,
	INGAME,
};

struct System {
	bool running;
	GameState state;
	System(bool running = true, GameState state = GameState::MAINMENU);
};

#endif