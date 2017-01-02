#pragma once

#include "ofMain.h"
#include "Tile.h"

class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void updateTile(int i);
    void draw();

    float secondsElapsed;
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


    // Scene 2
    //=================================
    float avoidanceDist;
    float avoidanceScalar;
    float cohesionDist;
    float cohesionScalar;
    float alignDist;
    float alignScalar;


    // Interaction
    //===================================

    void windowResized(int w, int h);
    void mousePressed(int x, int y, int button);
    void keyPressed(int key);

};
