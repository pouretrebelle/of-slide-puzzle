#include "Tile.h"
#include "ofxEasing.h"

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
		x = ofxeasing::map_clamp(curFrame, 0, animLength, startX, endX, &ofxeasing::quad::easeOut);
		y = ofxeasing::map_clamp(curFrame, 0, animLength, startY, endY, &ofxeasing::quad::easeOut);
		if (curFrame > animLength) moving = false;
	}
}

void Tile::draw() {
}
