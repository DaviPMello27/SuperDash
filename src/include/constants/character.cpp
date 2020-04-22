#include "structs/character.h"

Character createCharacterOlavo(Character olavo) {
	olavo = {
		"olavo",
		{
			2, //speed
			3, //dash speed
			3, //dash recovery
			2  //jump
		},
	}; //TODO: implement character data, turn into array
	return olavo;
}

Character createCharacterWroth(Character Wroth) {
	Wroth = {
		"wroth",
		{
			1, //speed
			5, //dash speed
			4, //dash recovery
			1  //jump
		},
	};
	return Wroth;
}