#include "Tile.h"


// Constructor
//=====================================

Tile::Tile(int _x, int _y, float _w, float _h, float _gutter) {
  x = _x;
  y = _y;
  initialX = _x;
  initialY = _y;
  w = _w;
  h = _h;
  gutter = _gutter;
  dotSize = 10;
  tileMoveAnimFrames = 10;
  tileSizeAnimFrames = 20;

  // initialise scene and transition array
  scene = 0;
  transitions = new float[2];

  // first transition is determined by secondsBetweenMoves and initialMoves
  // in ofApp.cpp, and the initial position of
  transitions[0] = 101 * 0.3 + (initialX + initialY) * 0.2;
  transitions[1] = transitions[0] + 5;
}


// Global Update
//=====================================

void Tile::update(int frameCounter, float secondsElapsed) {
  // increment current frame
  curFrame++;

  // set scene
  //-------------------------------------
  if (secondsElapsed > transitions[0] && scene < 1) {
    // scene 1
    scene = 1;
    curFrame = 0;
  }
  else if (secondsElapsed > transitions[1] && scene < 2) {
    // scene 2
    scene = 2;
    curFrame = 0;
    setupS2();
  }

  // update scene
  switch (scene) {
  case 0: updateS0(frameCounter); break;
  case 1: updateS1(frameCounter); break;
  case 2: updateS2(frameCounter); break;
  }
}


// Global Draw
//=====================================

void Tile::draw() {
  // draw scene
  switch (scene) {
  case 0: drawS0(); break;
  case 1: drawS1(); break;
  case 2: drawS2(); break;
  }
}


// Helper Functions
//=====================================

// functions borrowed from https://github.com/jesusgollonet/ofpennereasing
float Tile::easeOutCubic(float t, float b, float c, float d) {
  return c*((t = t / d - 1)*t*t + 1) + b;
}
float Tile::easeInCubic(float t, float b, float c, float d) {
  return c*(t /= d)*t*t + b;
}


// Scene 0
//=====================================

void Tile::updateS0(int frameCounter) {
  if (moving) {
    x = easeOutCubic(curFrame, startX, endX - startX, tileMoveAnimFrames);
    y = easeOutCubic(curFrame, startY, endY - startY, tileMoveAnimFrames);
    // if the animation animation is complete, set moving to false
    if (curFrame >= tileMoveAnimFrames) moving = false;
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

void Tile::drawS0() {
  // check to make sure the image has a texture
  if (image.getWidth() != 0) {
    image.draw(x*w, y*h);
  }
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


// Scene 1
//=====================================

void Tile::updateS1(int frameCounter) {
}

void Tile::drawS1() {
  float dotWidth = easeInCubic(curFrame, w-gutter, -w+gutter+dotSize, tileSizeAnimFrames);
  float dotHeight = easeInCubic(curFrame, h-gutter, -h+gutter+dotSize, tileSizeAnimFrames);
  // if the animation is complete clamp width and height
  if (curFrame > tileSizeAnimFrames) {
    dotWidth = dotSize;
    dotHeight = dotSize;
  }
  float dotX = (w - dotWidth) * 0.5;
  float dotY = (h - dotHeight) * 0.5;
  image.drawSubsection(x*w + dotX, y*h + dotY, dotWidth, dotHeight, dotX, dotY);
}



// Scene 2
//=====================================

void Tile::setupS2() {
}

void Tile::updateS2(int frameCounter) {
}

void Tile::drawS2() {
}
