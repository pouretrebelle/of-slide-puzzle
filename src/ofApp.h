#pragma once

#include "ofMain.h"
#include "Tile.h"

class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void updateTile(int i);
    void draw();

    bool animationBegan;

    float secondsElapsed;
    float secondsElapsedSinceBegan;
    float secondsElapsedLastMoved;
    float secondsBetweenMoves;

    vector<Tile> tiles;
    int frameCounter;
    int windowW, windowH;


    // Scene 0
    //===================================

    ofVideoGrabber vidGrabber;
    bool loopBack;
    int** tilePositions;
    int initialMoves;
    int tileGutter;
    ofVec2f blank;
    ofVec2f lastDirection;
    vector<ofVec2f> moves;
    ofImage image;
    int tilesX, tilesY;
    float tileW, tileH;
    ofVec2f getADirection();
    void move(ofVec2f direction);
    void initialiseMoves(int count);


    // Scene 4+6
    //=================================

    bool reinitialiseMoves;
    bool reactivateLoopBack;


    // Interaction
    //===================================

    void windowResized(int w, int h);
    void keyPressed(int key);

};
