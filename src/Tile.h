#pragma once
#include "ofMain.h"

class Tile {

public:

	Tile(int _x, int _y, float _w, float _h) {
		x = _x;
		y = _y;
		initialX = _x;
		initialY = _y;
		w = _w;
		w = _h;
	}

	void move(ofVec2f dir, bool easing);
	void update();
	void draw();

	bool moving = false;
	int initialX, initialY;
	int curFrame;
	int animLength = 10;
	float startX, startY, endX, endY;
	float x, y;
	float w, h;

private:

};