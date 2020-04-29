#include "genericFunctions/rand.h"

int randint(int min, int max) {
	srand(time(NULL));
	return (rand() % max - min) + min;
}