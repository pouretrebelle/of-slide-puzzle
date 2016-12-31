#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	frameCounter = 0;
	ofSetFrameRate(60);
	ofBackground(255);

	initialMoves = 100;
	tileGutter = 10;

	windowW = ofGetWidth() - tileGutter;
	windowH = ofGetHeight() - tileGutter;
	tilesX = 8;
	tilesY = 6;
	tileW = windowW / tilesX;
	tileH = windowH / tilesY;

	blank = ofVec2f(0, 0);

	// initialise tile positions array
	tilePositions = new int*[tilesX];
	for (int x = 0; x < tilesX; x++) {
		tilePositions[x] = new int[tilesY];
	}

	// initialise tiles
	for (int y = 0; y < tilesY; y++) {
		for (int x = 0; x < tilesX; x++) {
			Tile newTile(x, y, tileW, tileH);
			tiles.push_back(newTile);
			tilePositions[x][y] = y * tilesX + x;
		}
	}

	// set up the camera
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(1);
	vidGrabber.initGrabber(640, 480);

	// initialise the first x moves and the looper
	initialiseMoves(initialMoves);
	loopBack = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	cout << ofGetFrameRate() << endl;
	frameCounter++;

	// update the image frame
	//if (frameCounter % 30 == 0) {
		vidGrabber.update();
		if (vidGrabber.isFrameNew()) {
			ofImage newImage;
			newImage.allocate(640, 480, OF_IMAGE_COLOR);
			newImage.setFromPixels(vidGrabber.getPixels());
			newImage.mirror(false, true);
			newImage.resize(windowW, windowH);
			image = newImage;
		}
	//}

	// loop back
	if (frameCounter % 10 == 0) {
		if (loopBack && moves.size()) {
			move(-moves.back());
			moves.pop_back();
		}
		else if (loopBack) {
			loopBack = !loopBack;
		}
	}

	// call update on all the tiles
	for (int i = 1; i < tilesY*tilesX; i++) {
		tiles[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofTranslate(tileGutter, tileGutter);

	// never draw the first tile, it's effecively our blank tile
	for (int i = 1; i < tilesY*tilesX; i++) {
		Tile tile = tiles[i];

		if (tile.updateImage) {
			ofImage tileImage;
			tileImage.cropFrom(image, tile.initialX*tileW, tile.initialY*tileH, tileW - tileGutter, tileH - tileGutter);
			tile.image = tileImage;
		}

		tile.draw();
	}
}

void ofApp::move(ofVec2f direction = ofVec2f(0, 0)) {
	bool easing = true;
	// if the direction isn't defined make one
	// and add it to the moves array
	if (direction.length() == 0) {
		direction = getADirection();
		moves.push_back(direction);
		easing = false;
	}
	int currentX = blank.x;
	int currentY = blank.y;
	blank += direction;
	int targetX = blank.x;
	int targetY = blank.y;
	int targetTileI = tilePositions[targetX][targetY];
	tilePositions[currentX][currentY] = targetTileI;
	tilePositions[targetX][targetY] = 0;
	tiles[targetTileI].move(direction, easing);
}

ofVec2f ofApp::getADirection() {
	// returns a unit ofVec2f based on moveable direction
	vector<ofVec2f> directions;
	
	ofVec2f west(-1, 0);
	if (blank.x > 0) directions.push_back(west);

	ofVec2f north(0, -1);
	if (blank.y > 0) directions.push_back(north);
	
	ofVec2f east(1, 0);
	if (blank.x < tilesX - 1) directions.push_back(east);
	
	ofVec2f south(0, 1);
	if (blank.y < tilesY - 1) directions.push_back(south);
	
	ofVec2f direction = directions[ofRandom(directions.size())];

	// don't let it go back on itself
	if (direction == -lastDirection) direction = getADirection();

	lastDirection.set(direction);
	return direction;
}

void ofApp::initialiseMoves(int count) {
	for (int i = 0; i < count; i++) {
		move();
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	move(getADirection());
}
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		loopBack = !loopBack;
	}
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	windowW = ofGetWidth();
	windowH = ofGetHeight();
}
