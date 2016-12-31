#include "Tile.h"

Tile::Tile(int _x, int _y, float _w, float _h) {
	x = _x;
	y = _y;
	initialX = _x;
	initialY = _y;
	w = _w;
	h = _h;

	// initialise scene and transition array
	scene = 0;
	transitions = new float[2];

	// first transition is determined by secondsBetweenMoves and initialMoves
	// in ofApp.cpp, and the initial position of
	transitions[0] = 100 * 0.3 + (initialX + initialY) * 0.2;
	transitions[1] = transitions[0] + 5;
}

void Tile::move(ofVec2f dir, bool easing) {
	// if easing is true initialise moving and set start and end
	if (easing) {
		moving = true;
		startX = x;
		startY = y;
		endX = x - dir.x;
		endY = y - dir.y;
		curFrame = 0;
	}
	// otherwise jump to new position
	else {
		x -= dir.x;
		y -= dir.y;
	}
}

void Tile::update(int frameCounter, float secondsElapsed) {
	// increment current frame
	curFrame++;

	// set scene
	if (secondsElapsed > transitions[0] && scene < 1) {
		scene = 1;
		curFrame = 0;
	}

	// update scene
	switch (scene) {
		case 0: updateS0(frameCounter); break;
	}
}

void Tile::updateS0(int frameCounter) {
	if (moving) {
		x = easeOutCubic(curFrame, startX, endX - startX, animLength);
		y = easeOutCubic(curFrame, startY, endY - startY, animLength);
		// if the animation animation is complete, set moving to false
		if (curFrame >= animLength) moving = false;
	}

	// update tile based on its initial position
	// creates diagonal sequential update pattern
	// based on 6*8 grid
	if (frameCounter % (6+8) == (initialX + initialY - 1)) {
		updateImage = true;
	}
	// update all of them for the first 5 frames
	// to assure there is an image for each tile
	else if (frameCounter > 5) {
		updateImage = false;
	}
	else {
		updateImage = true;
	}
}

void Tile::draw() {
	// draw scene
	switch (scene) {
		case 0: drawS0(); break;
	}
}

void Tile::drawS0() {
	// check to make sure the image has a texture
	if (image.getWidth() != 0) {
		image.draw(x*w, y*h);
	}
}

// function borrowed from https://github.com/jesusgollonet/ofpennereasing
float Tile::easeOutCubic(float t, float b, float c, float d) {
	return c*((t = t / d - 1)*t*t + 1) + b;
}
