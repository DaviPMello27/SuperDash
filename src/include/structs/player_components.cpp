#include "structs/player_components.h"

void Animation::walk(bool condition, int limit, int variation) {
	src = { offset, 0, 24, 48 };
	if (condition) {
		counter++;
		if (counter % 10 == 0) {
			offset += variation;
		}
		if (offset == limit) {
			offset = variation;
		}
	}
	else {
		counter = 0;
		offset = 0;
	}
}
void Animation::jump(float ySpeed) {
	if (ySpeed < -1.5) {
		offset = 168;
	}
	else if (ySpeed >= -1.5 && ySpeed < 1.5) {
		offset = 192;
	}
	else {
		offset = 216;
	}
	src = { offset, 0, 24, 48 };
}

void Animation::death(float ySpeed, int addOffset) {
	if (ySpeed < -1.5) {
		offset = 240 + addOffset;
	}
	else if (ySpeed == 0) {
		offset = 312 + addOffset;
	}
	else if (ySpeed >= -1.5 && ySpeed < 1.5) {
		offset = 264 + addOffset;
	}
	else if (ySpeed >= 1.5) {
		offset = 288 + addOffset;
	}
	src = { offset, 0, 24, 48 };
}

void Animation::dash(int cooldown, double maxSpeed) {
	if (cooldown > 45 || cooldown < 10) {
		offset = 0;
	}
	else if (cooldown > 40 || cooldown < 20) {
		offset = 72;
	}
	else {
		offset = 144 + 72 * (cooldown % 4);
	}
	src = { offset, 48, 72, 24 };
}