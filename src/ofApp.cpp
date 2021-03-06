#include "ofApp.h"

// Setup
//=====================================

void ofApp::setup(){
  ofSetFrameRate(30);
  ofSetCircleResolution(100);
  ofBackground(255);

  animationBegan = false;

  // set timers
  secondsElapsed = 0;
  secondsElapsedSinceBegan = 0;
  secondsElapsedLastMoved = 0;
  frameCounter = 0;

  // position variables
  tileGutter = 10;
  windowW = ofGetWidth() - tileGutter;
  windowH = ofGetHeight() - tileGutter;
  tilesX = 8;
  tilesY = 6;
  tileW = windowW / tilesX;
  tileH = windowH / tilesY;
  blank = ofVec2f(0, 0);

  // scene-relevent variables
  reinitialiseMoves = false;
  reactivateLoopBack = false;

  // animation variables
  secondsBetweenMoves = 0.3;
  initialMoves = 140;
  scene7Moves = 135;

  // initialise tile positions array
  tilePositions = new int*[tilesX];
  for (int x = 0; x < tilesX; x++) {
    tilePositions[x] = new int[tilesY];
  }

  // initialise tiles
  for (int y = 0; y < tilesY; y++) {
    for (int x = 0; x < tilesX; x++) {
      Tile newTile(x, y, tileW, tileH, tileGutter);
      tiles.push_back(newTile);
      tilePositions[x][y] = y * tilesX + x;
    }
  }

  // set up the camera
  vidGrabber.setDeviceID(0);
  vidGrabber.setDesiredFrameRate(10);
  vidGrabber.initGrabber(640, 480);
  // open video settings on start
  vidGrabber.videoSettings();

  // initialise the first x moves and the looper
  initialiseMoves(initialMoves);
  loopBack = true;
}


// Global Update
//=====================================

void ofApp::update(){
  // set seconds elapsed
  secondsElapsed = ofGetElapsedTimeMillis() * 0.001 - secondsElapsedSinceBegan;

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

  // call update on all the tiles
  // apart from the first one, we don't care about that one
  for (int i = 1; i < tilesY*tilesX; i++) {
    updateTile(i);
  }

  // stop updating from this line if the animation hasn't begun
  if (!animationBegan) return;


  // Frame things
  //-----------------------------------

  // log the frame rate
  cout << ofGetFrameRate() << endl;

  // increment the frame counter
  frameCounter++;


  // Timing things
  //-----------------------------------

  // reinitialise moves after Scene 3
  if (secondsElapsed > 92 && !reinitialiseMoves) {
    initialiseMoves(scene7Moves);
    reinitialiseMoves = true;
  }

  // reactivate loopBack for Scene 6
  if (secondsElapsed > 136 && !reactivateLoopBack) {
    loopBack = true;
    reactivateLoopBack = true;
  }

  // exit program if it's been more than 3 minutes
  if (secondsElapsed > 3 * 60) ofExit();


  // Undo existing moves every secondsBetweenMoves
  //-----------------------------------

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

}


// Global Draw
//=====================================

void ofApp::draw(){
  ofTranslate(tileGutter, tileGutter);

  // never draw the first tile, it's effecively our blank tile
  for (int i = 1; i < tilesY*tilesX; i++) {
    tiles[i].draw();
  }
}


// Tile Update
//=====================================

void ofApp::updateTile(int i) {
  // if the tile image should be updated
  if (tiles[i].updateImage == true) {
    ofImage tileImage;
    tileImage.allocate(tileW, tileH, OF_IMAGE_COLOR);
    tileImage.cropFrom(image, tiles[i].initialX*tileW, tiles[i].initialY*tileH, tileW - tileGutter, tileH - tileGutter);
    tiles[i].image.clone(tileImage);
    tiles[i].targetColor = tileImage.getColor(tileW*0.5, tileH*0.5);
  }

  // call update on the tiles
  tiles[i].update(frameCounter, secondsElapsed);

  // Boiding
  //-----------------------------------
  if (tiles[i].boiding) {

    ofVec2f pos = tiles[i].pos;
    ofVec2f seperate = ofVec2f(0, 0);
    ofVec2f cohesion = ofVec2f(0, 0);
    ofVec2f align = ofVec2f(0, 0);
    int numNeighboursCohesion = 0;
    int numNeighboursAlign = 0;
    float closestDistance = windowW;

    // loop through others
    for (int j = 1; j < tilesY*tilesX; j++) {
      // get distance between dots
      float totalDist = pos.distance(tiles[j].pos);
      // and between dot edges
      float dist = totalDist;
      dist -= tiles[i].dotSize*0.5;
      dist -= tiles[j].dotSize*0.5;

      // if target is boiding and not itself
      if (tiles[j].boiding && j != i) {

        // Seperate
        if (dist < tiles[i].avoidanceDist) {
          ofVec2f reverseDiff = (tiles[j].pos - pos);
          reverseDiff.scale(tiles[i].avoidanceDist-dist);
          seperate -= reverseDiff;

          if (ofRandom(1) < 0.01) cout << dist << endl;
        }

        // Cohesion
        if (dist < tiles[i].cohesionDist) {
          cohesion += tiles[j].pos;
          numNeighboursCohesion++;
        }

        // Align
        if (dist < tiles[i].alignDist) {
          align += tiles[j].vel;
          numNeighboursAlign++;
        }
      }

      // if it's scene 3-4 and not itself
      if (tiles[i].scene > 2 && tiles[i].scene < 5 && j != i) {
        if (closestDistance > totalDist) {
          closestDistance = totalDist;
        }
      }
    }

    // Seperate
    seperate *= tiles[i].avoidanceScalar;
    tiles[i].vel += seperate;

    // Cohesion
    if (numNeighboursCohesion > 0) {
      cohesion /= numNeighboursCohesion;
      cohesion -= pos;
      cohesion *= tiles[i].cohesionScalar;
      tiles[i].vel += cohesion;
    }

    // Align
    if (numNeighboursAlign > 0) {
      align /= numNeighboursAlign;
      align *= tiles[i].alignScalar;
      tiles[i].vel += align;
    }

    // Scene 3-4 closest dot
    tiles[i].dotSizeTarget = closestDistance - tileGutter;
  }
}


// Helper Functions
//=====================================

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


// Interaction
//=====================================

// Space bar to begin animation
//-------------------------------------

void ofApp::keyPressed(int key) {
  if (key == ' ' && !animationBegan) {
    animationBegan = true;
    secondsElapsedSinceBegan = secondsElapsed;
  }
}

// Window Resize
//-------------------------------------

void ofApp::windowResized(int w, int h){
  windowW = ofGetWidth();
  windowH = ofGetHeight();
}
