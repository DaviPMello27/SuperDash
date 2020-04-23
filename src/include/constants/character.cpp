#include "structs/character.h"

//ID 1 = Olavo
//ID 2 = Wroth

Character::Character(int ID) {
	if (ID == 1) {
		this->name = "Olavo";
		this->stat =
		{
			2, //speed
			3, //dash speed
			3, //dash recovery
			2  //jump
		};
	};
	if (ID == 2) {
		this->name = "Wroth";
		this->stat =
		{
			1, //speed
			5, //dash speed
			4, //dash recovery
			1  //jump
		};
	};
}