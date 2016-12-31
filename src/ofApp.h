#pragma once

#include "ofMain.h"
#include "Tile.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void move(ofVec2f direction);

		void initialiseMoves(int count);

		void windowResized(int w, int h);
		void mousePressed(int x, int y, int button);
		void keyPressed(int key);

		ofVec2f getADirection();

		ofVideoGrabber vidGrabber;

		float secondsElapsed;
		float secondsElapsedLastMoved;
		float secondsBetweenMoves;

		vector<Tile> tiles;
		int** tilePositions;
		ofVec2f blank;
		ofVec2f lastDirection;
		vector<ofVec2f> moves;
		ofImage image;
		int frameCounter;
		int initialMoves;
		int tileGutter;
		int windowW, windowH;
		int tilesX, tilesY;
		float tileW, tileH;

		bool loopBack;
};
