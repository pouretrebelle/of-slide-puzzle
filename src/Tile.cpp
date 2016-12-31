#include "Tile.h"

Tile::Tile(int _x, int _y, float _w, float _h) {
	x = _x;
	y = _y;
	initialX = _x;
	initialY = _y;
	w = _w;
	h = _h;
}

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

void Tile::update(int frameCounter) {
	if (moving) {
		curFrame++;
		x = easeOutCubic(curFrame, startX, endX-startX, animLength);
		y = easeOutCubic(curFrame, startY, endY-startY, animLength);
		if (curFrame >= animLength) moving = false;
	}

	// update tile based on its initial position
	// creates diagonal sequential update pattern
	// based on 6*8 grid
	if (frameCounter % (6+8) == (initialX + initialY - 1)) {
		updateImage = true;
	}
	// update all of them for the first 10 frames
	// to assure there is an image for each tile
	else if (frameCounter > 5) {
		updateImage = false;
	}
	else {
		updateImage = true;
	}
}

void Tile::draw() {
	// check to make sure the image has a texture
	if (image.getWidth() != 0) {
		image.draw(x*w, y*h);
	}
}

// function borrowed from https://github.com/jesusgollonet/ofpennereasing
float Tile::easeOutCubic(float t, float b, float c, float d) {
	return c*((t = t / d - 1)*t*t + 1) + b;
}
