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

  // initialise scene and transition array
  scene = 0;
  transitions = new float[2];

  // first transition is determined by secondsBetweenMoves and initialMoves
  // in ofApp.cpp, and the initial position of
  transitions[0] = 101 * 0.3 + (initialX + initialY) * 0.3; // ~32 seconds
  transitions[1] = transitions[0] + 3; // ~35 seconds
  transitions[2] = 60 + (initialX + initialY) * 1; // 60-75 seconds
  transitions[3] = 90 + (initialX + initialY) * 1; // 90-105 seconds

  // Scene 0
  //-----------------------------------
  tileMoveAnimFrames = 10;

  // Scene 1
  //-----------------------------------
  tileSizeAnimFrames = 30;
  dotSize = 10;
  dotSizeTarget = dotSize;

  // Scene 2
  //-----------------------------------
  speed = 3;
  speedMax = 0;
  speedMaxIncrement = 0.05;
  speedMaxLimit = 4;
  colorLerpScalar = 0.1;
  avoidanceDist = 40;
  avoidanceScalar = 0.01;
  cohesionDist = 80;
  cohesionScalar = 0.005;
  alignDist = 60;
  alignScalar = 0.01;
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
  else if (secondsElapsed > transitions[2] && scene < 3) {
    // scene 3
    scene = 3;
    curFrame = 0;
    setupS3();
  }
  else if (secondsElapsed > transitions[3] && scene < 4) {
    // scene 4
    scene = 4;
    curFrame = 0;
    setupS4();
  }

  // update scene
  switch (scene) {
  case 0: updateS0(frameCounter); break;
  case 1: updateS1(frameCounter); break;
  case 2: updateS2(frameCounter); break;
  case 3: updateS3(frameCounter); break;
  case 4: updateS4(frameCounter); break;
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
  case 3: drawS3(); break;
  case 4: drawS4(); break;
  }
}


// Shared Functions
//=====================================

// Image Updates - every scene
//-------------------------------------
void Tile::updateImageSometimes(int frameCounter) {
  // update tile based on its initial position
  // creates diagonal sequential update pattern
  // based on 6*8 grid
  if (frameCounter % (6 + 8) == (initialX + initialY - 1)) {
    updateImage = true;
  }
  else {
    updateImage = false;
  }
}

// Boid Updates - scene 2-3
//-------------------------------------
void Tile::updateBoids(int frameCounter) {
  // bounce off borders
  // and push position to stop getting stuck inside edge
  if (pos.x < dotSize) {
    // top
    vel.x *= -1;
    pos.x++;
  }
  else if (pos.x > ofGetWindowWidth()-dotSize) {
    // bottom
    vel.x *= -1;
    pos.x--;
  }
  if (pos.y < dotSize) {
    // left
    vel.y *= -1;
    pos.y++;
  }
  else if (pos.y > ofGetWindowHeight() - dotSize) {
    // right
    vel.y *= -1;
    pos.y--;
  }

  // clamp velocity
  ofVec2f clampedVel = vel;
  if (clampedVel.length() > speedMax) {
    clampedVel.scale(speedMax);
  }

  // update position
  pos += clampedVel;

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  color.lerp(targetColor, colorLerpScalar);

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  color.lerp(targetColor, colorLerpScalar);
}

// Draw Square/Circle hybrid - scene 3
//-------------------------------------
// from example in https://forum.processing.org/two/discussion/1941/transform-a-square-into-a-circle
void Tile::drawSquircle(float radius, float t) {
  // square for t = 0
  // circle for t = 1
  float circleX, circleY, squareX, squareY;
  ofPath squircle;

  for (float angle = DEG_TO_RAD*5; angle < TWO_PI; angle += DEG_TO_RAD*10) {
    circleX = radius*cos(angle);
    circleY = radius*sin(angle);

    if ((angle > DEG_TO_RAD*225) && (angle <= DEG_TO_RAD*315)) {
      // top side
      float squareRadius = radius / sin(angle);
      squareX = - squareRadius*cos(angle);
      squareY = - radius;
    }
    else if ((angle > DEG_TO_RAD*45) && (angle <= DEG_TO_RAD*135)) {
      // bottom side
      float squareRadius = radius / sin(angle);
      squareX = squareRadius*cos(angle);
      squareY = radius;
    }
    else if ((angle > DEG_TO_RAD*135) && (angle <= DEG_TO_RAD*225)) {
      // left side
      float squareRadius = radius / cos(angle);
      squareX = - radius;
      squareY = - squareRadius*sin(angle);
    }
    else {
      // right side
      float squareRadius = radius / cos(angle);
      squareX = radius;
      squareY = squareRadius*sin(angle);
    }
    squircle.lineTo(ofLerp(squareX, circleX, t), ofLerp(squareY, circleY, t));
  }

  squircle.setFillColor(color);
  squircle.draw();
}


// Scene 0
//=====================================
// solve puzzle

void Tile::updateS0(int frameCounter) {
  if (moving) {
    x = easeOutCubic(curFrame, startX, endX - startX, tileMoveAnimFrames);
    y = easeOutCubic(curFrame, startY, endY - startY, tileMoveAnimFrames);
    // if the animation animation is complete, set moving to false
    if (curFrame >= tileMoveAnimFrames) moving = false;
  }

  updateImageSometimes(frameCounter);
  // apart from first 5 frames
  // to assure there is an image for each tile
  if (frameCounter < 5) {
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
// reduce to dots

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
  float dotX = (w - dotWidth - gutter) * 0.5;
  float dotY = (h - dotHeight - gutter) * 0.5;
  image.drawSubsection(x*w + dotX, y*h + dotY, dotWidth, dotHeight, dotX, dotY);
}


// Scene 2
//=====================================
// boid about

void Tile::setupS2() {
  // setup position
  pos.x = x*w + (w-gutter)*0.5;
  pos.y = y*h + (h-gutter)*0.5;

  // get color from image
  // targetColor is updated in ofApp's updateTile()
  color = targetColor;

  // set velocity with speed
  // randomise direction to each diagonal
  vel = ofVec2f(speed / sqrt(2));
  vel.rotate(90 * floor(ofRandom(4)));
}

void Tile::updateS2(int frameCounter) {
  // update maximum speed
  if (speedMax < speedMaxLimit) {
    speedMax += speedMaxIncrement;
  }
  updateBoids(frameCounter);
}

void Tile::drawS2() {
  ofSetColor(color);
  ofPushMatrix();

  // translate to position
  ofTranslate(pos.x, pos.y);

  // rotate context to velocity minus 45deg
  // so corner of rectangle always points forward
  ofRotate(ofVec2f(1, -1).angle(vel));

  ofDrawRectangle(-dotSize*0.5, -dotSize*0.5, dotSize, dotSize);
  ofPopMatrix();
}


// Scene 3
//=====================================
// grow into circles

void Tile::setupS3() {
  squircleness = 0;
  squirclenessIncrement = 0.02;
  dotSizeIncrement = 0;

  // up avoidance scalar to avoid overlapping
  avoidanceScalar = 0.5;
  avoidanceDistIncrement = 0.1;
}

void Tile::updateS3(int frameCounter) {
  updateBoids(frameCounter);

  // circlify dots
  if (squircleness < 1) {
    squircleness += squirclenessIncrement;
  }

  // accelerate dot size
  if (dotSizeIncrement < 0.2) {
    dotSizeIncrement += 0.005;
  }

  // lerp dot size towards target if it's smaller than the target
  if (dotSize < dotSizeTarget) {
    dotSize = ofLerp(dotSize, dotSizeTarget, dotSizeIncrement);
  }
  // otherwise jump to target
  // to avoid overlap
  else {
    dotSize = dotSizeTarget;
  }

  // lerp avoidance distance towards dot size
  // to encourage biggest dots possible
  avoidanceDist = ofLerp(avoidanceDist, dotSize+gutter*0.5, avoidanceDistIncrement);
}

void Tile::drawS3() {
  ofPushMatrix();

  // translate to position
  ofTranslate(pos.x, pos.y);

  // rotate context to velocity minus 45deg
  // so corner of rectangle always points forward
  ofRotate(ofVec2f(1, -1).angle(vel));

  drawSquircle(dotSize*0.5, squircleness);
  ofPopMatrix();
}


// Scene 4
//=====================================
// reform grid

void Tile::setupS4() {
}

void Tile::updateS4(int frameCounter) {
}

void Tile::drawS4() {
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
