#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(255);

	tileGutter = 10;
	windowW = ofGetWidth() - tileGutter;
	windowH = ofGetHeight() - tileGutter;
	tilesX = 8;
	tilesY = 6;
	tileW = windowW / tilesX;
	tileH = windowH / tilesY;

	blank = ofVec2f(0, 0);

	// animation variables
	secondsBetweenMoves = 0.3;
	initialMoves = 100;

	// set timers
	secondsElapsed = 0;
	secondsElapsedLastMoved = 0;
	frameCounter = 0;

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
	// set seconds elapsed
	secondsElapsed = ofGetElapsedTimeMillis() * 0.001;
	// exit program if it's been more than 3 minutes
	if (secondsElapsed > 3 * 60) ofExit();

	// log the frame rate
	cout << ofGetFrameRate() << endl;

	// increment the frame counter
	frameCounter++;

	// update the image frame
	vidGrabber.update();
	if (vidGrabber.isFrameNew()) {
		ofImage newImage;
		newImage.allocate(640, 480, OF_IMAGE_COLOR);
		newImage.setFromPixels(vidGrabber.getPixels());
		newImage.mirror(false, true);
		newImage.resize(windowW, windowH);
		image = newImage;
	}

	// undo existing moves
	// every secondsBetweenMoves
	if (secondsElapsedLastMoved < (secondsElapsed - secondsBetweenMoves)) {
		// if loopBack is true and there are moves
		if (loopBack && moves.size()) {
			// move the reverse vector of the last move
			move(-moves.back());
			// remove the last move from the array
			moves.pop_back();
		}
		// no more moves
		else if (loopBack) {
			loopBack = false;
		}
		// increment last moved variable
		secondsElapsedLastMoved += secondsBetweenMoves;
	}

	// call update on all the tiles
	// apart from the first one, we don't care about that one
	for (int i = 1; i < tilesY*tilesX; i++) {

		tiles[i].update(frameCounter);

		// if the tile image should be updated
		if (tiles[i].updateImage == true) {
			ofImage tileImage;
			tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
			tileImage.cropFrom(image, tiles[i].initialX*tileW, tiles[i].initialY*tileH, tileW - tileGutter, tileH - tileGutter);
			tiles[i].image.clone(tileImage);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofTranslate(tileGutter, tileGutter);

	// never draw the first tile, it's effecively our blank tile
	for (int i = 1; i < tilesY*tilesX; i++) {
		tiles[i].draw();
	}

	// draw progress through animation in seconds
	ofDrawBitmapString(ofToString(secondsElapsed), 20, 20);
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
