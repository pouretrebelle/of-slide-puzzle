#include "Tile.h"

void Tile::move(ofVec2f dir, bool easing) {
	if (easing) {
		moving = true;
		startX = x;
		startY = y;
		endX = x - dir.x;
		endY = y - dir.y;
		curFrame = 0;
	}
	else {
		x -= dir.x;
		y -= dir.y;
	}
}

void Tile::update() {
	if (moving) {
		curFrame++;
		x = easeOutCubic(curFrame, startX, endX-startX, animLength);
		y = easeOutCubic(curFrame, startY, endY-startY, animLength);
		if (curFrame >= animLength) moving = false;
	}
}

void Tile::draw() {
}

// function borrowed from https://github.com/jesusgollonet/ofpennereasing
float Tile::easeOutCubic(float t, float b, float c, float d) {
	return c*((t = t / d - 1)*t*t + 1) + b;
}
