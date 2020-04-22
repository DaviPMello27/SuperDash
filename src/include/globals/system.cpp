#include "structs/system.h"


System dashSystem(System system) {
	system = { true, GAMESTATE_MAINMENU };
	return system;
}