#include <random>

int randint(int min, int max) {
	return (rand() % max - min) + min;
}