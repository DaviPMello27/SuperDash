#ifndef DASHSCREEN_H
#define DASHSCREEN_H

struct Size {
	int w;
	int h;
};

struct Scale {
	float w;
	float h;
};

struct Screen {
	Size size;
	Scale scale;
};

#endif

