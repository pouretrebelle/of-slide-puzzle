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

  // initialise transition array
  transitions = new float[5];

  // first transition is determined by secondsBetweenMoves and initialMoves
  // in ofApp.cpp, and it initial tile position
  transitions[0] = 140 * 0.3 + (initialX + initialY) * 0.3; // 42 - 46 seconds
  transitions[1] = transitions[0] + 2 + (initialX + initialY) * 0.2; // 44 - 47 seconds
  transitions[2] = 62 + (initialX + initialY) * 2; // 62-92 seconds
  transitions[3] = 117 - (initialX + initialY) * 1; // 102-117 seconds
  transitions[4] = 126 - (initialX*0.8 + initialY) * 1.2; // 110 - 126 seconds
  transitions[5] = 136 - (initialX*0.8 + initialY) * 1.2; // 120 - 136 seconds
  transitions[6] = 136; // 136 seconds

  // controls whether ofApp.cpp compares tiles
  boiding = false;

  // set up first scene
  setupS0();
}


// Global Update
//=====================================

void Tile::update(int frameCounter, float secondsElapsed) {
  // increment current frame
  curFrame++;

  // setup scene
  //-------------------------------------
  if (secondsElapsed > transitions[0] && scene < 1) {
    // scene 1
    setupS1();
  }
  else if (secondsElapsed > transitions[1] && scene < 2) {
    // scene 2
    setupS2();
  }
  else if (secondsElapsed > transitions[2] && scene < 3) {
    // scene 3
    setupS3();
  }
  else if (secondsElapsed > transitions[3] && scene < 4) {
    // scene 4
    setupS4();
  }
  else if (secondsElapsed > transitions[4] && scene < 5) {
    // scene 5
    setupS5();
  }
  else if (secondsElapsed > transitions[5] && scene < 6) {
    // scene 6
    setupS6();
  }
  else if (secondsElapsed > transitions[6] && scene < 7) {
    // scene 7
    setupS7();
  }

  // update scene
  switch (scene) {
  case 0: updateS0(frameCounter, secondsElapsed); break;
  case 1: updateS1(frameCounter); break;
  case 2: updateS2(frameCounter); break;
  case 3: updateS3(frameCounter); break;
  case 4: updateS4(frameCounter); break;
  case 5: updateS5(frameCounter); break;
  case 6: updateS6(frameCounter); break;
  case 7: updateS7(frameCounter); break;
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
  case 5: drawS5(); break;
  case 6: drawS6(); break;
  case 7: drawS7(); break;
  }
}


// Shared Functions
//=====================================

// Image Updates - every scene
// not in global update because different often variable
//-------------------------------------
void Tile::updateImageSometimes(int frameCounter, int often = 20) {
  // update tile based on its initial position
  // creates diagonal sequential update pattern
  // often is how many frames per update
  // default to 20 for dot colour update
  if ((frameCounter - (initialX + initialY)) % often == 0) {
    updateImage = true;
  }
  else {
    updateImage = false;
  }
}

// Lerp Colour - scenes 2-5
//-------------------------------------
void Tile::lerpColor() {
  color.lerp(targetColor, colorLerpScalar);
}

// Boid Updates - scene 2-4
//-------------------------------------
void Tile::updateBoids(int frameCounter) {
  // bounce off borders
  // global app draw translates by (gutter, gutter) so we have to allow for that
  if (pos.x < dotSize*0.5 - gutter) {
    // left
    vel.x += 5;
    pos.x = dotSize*0.5 - gutter;
  }
  else if (pos.x > ofGetWindowWidth()-dotSize*0.5 - gutter) {
    // right
    vel.x -= 5;
    pos.x = ofGetWindowWidth()-dotSize*0.5 - gutter;
  }
  if (pos.y < dotSize*0.5 - gutter) {
    // top
    vel.y += 5;
    pos.y = dotSize*0.5 - gutter;
  }
  else if (pos.y > ofGetWindowHeight()-dotSize*0.5 - gutter) {
    // bottom
    vel.y -= 5;
    pos.y = ofGetWindowHeight()-dotSize*0.5 - gutter;
  }

  // clamp velocity
  ofVec2f clampedVel = vel;
  if (clampedVel.length() > speedMax) {
    clampedVel.scale(speedMax);
  }
  // check that max speed is more than min
  // because it's incremented from 0 for initial acceleration
  else if (speedMax > speedMin && clampedVel.length() < speedMin) {
    clampedVel.scale(speedMin);
  }

  // update position
  pos += clampedVel;
}

// Draw Square/Circle hybrid - scene 3+5
//-------------------------------------
// from example in https://forum.processing.org/two/discussion/1941/transform-a-square-into-a-circle
ofPath Tile::pathSquircle(float radius, float t) {
  // square for t = 0
  // circle for t = 1
  float circleX, circleY, squareX, squareY;
  ofPath squircle;

  for (float angle = DEG_TO_RAD*5; angle < TWO_PI; angle += DEG_TO_RAD*10) {
    circleX = radius + radius*cos(angle);
    circleY = radius + radius*sin(angle);

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
    squareX += radius;
    squareY += radius;
    squircle.lineTo(ofLerp(squareX, circleX, t), ofLerp(squareY, circleY, t));
  }
  squircle.close();
  return squircle;
}


// Scene 0
//=====================================
// solve puzzle

void Tile::setupS0() {
  scene = 0;
  tileMoveAnimFrames = 10;
  curFrame = 0;
}

void Tile::updateS0(int frameCounter, float secondsElapsed = 0) {
  if (moving) {
    x = easeOutCubic(curFrame, startX, endX - startX, tileMoveAnimFrames);
    y = easeOutCubic(curFrame, startY, endY - startY, tileMoveAnimFrames);
    // if the animation animation is complete, set moving to false
    if (curFrame >= tileMoveAnimFrames) moving = false;
  }

  // update image every 5 frames
  updateImageSometimes(frameCounter, 5);
  // apart from first 5 frames
  // to assure there is an image for each tile
  if (frameCounter < 5) {
    updateImage = true;
  }
}

void Tile::drawS0() {
  // reset color before drawing
  ofSetColor(ofColor::white);
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

void Tile::setupS1() {
  scene = 1;

  tileSizeAnimFrames = 30;
  dotSize = 14;
  dotSizeTarget = dotSize;
  curFrame = 0;

  // make sure the x+y are positioned absolutely
  x = initialX;
  y = initialY;
  // turn off any continuing animation
  moving = false;
}

void Tile::updateS1(int frameCounter) {
  // update image every 5 frames
  updateImageSometimes(frameCounter, 5);
}

void Tile::drawS1() {
  // reset color before drawing
  ofSetColor(ofColor::white);

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
  scene = 2;

  speed = 3;
  speedMin = 2;
  speedMax = 0;
  speedMaxIncrement = 0.1;
  speedMaxLimit = 6;
  colorLerpScalar = 0.1;
  avoidanceDist = 10;
  avoidanceScalar = 0.02;
  cohesionDist = 20;
  cohesionScalar = 0.005;
  alignDist = 30;
  alignScalar = 0.01;

  // setup position
  pos.x = x*w + (w-gutter)*0.5;
  pos.y = y*h + (h-gutter)*0.5;

  // make ofApp.cpp start comparing tiles
  boiding = true;

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

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  lerpColor();
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
  scene = 3;

  squircleness = 0;
  squirclenessIncrement = 0.02;
  dotSizeIncrement = 0;
  dotSizeMin = 30;

  // up avoidance scalar to avoid overlapping
  avoidanceScalar = 0.1;
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

  // lerp towards target, make sure it's > dotSizeMin
  dotSizeTarget = (dotSizeTarget < dotSizeMin) ? dotSizeMin : dotSizeTarget;
  dotSize = ofLerp(dotSize, dotSizeTarget, dotSizeIncrement);

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  lerpColor();
}

void Tile::drawS3() {
  ofPushMatrix();

  // translate to position
  ofTranslate(pos.x, pos.y);

  // rotate context to velocity minus 45deg
  // so corner of rectangle always points forward
  ofRotate(ofVec2f(1, -1).angle(vel));

  // translate again because squircles are drawn from the top left
  ofTranslate(-dotSize*0.5, -dotSize*0.5);

  squircle = pathSquircle(dotSize*0.5, squircleness);
  squircle.setFillColor(color);
  squircle.draw();

  ofPopMatrix();
}


// Scene 4
//=====================================
// reform grid

void Tile::setupS4() {
  scene = 4;

  posTargetAchieved = false;
  posTargetLerp = 0.01;
  posTargetIncrement = 0.001;
  posTarget.x = x*w + (w-gutter)*0.5;
  posTarget.y = y*h + (h-gutter)*0.5;
  avoidanceScalar = 0.1;
}

void Tile::updateS4(int frameCounter) {
  if (!posTargetAchieved && (posTargetLerp > 0.5 || pos.distance(posTarget) < 5)) {
    vel.x = 0;
    vel.y = 0;

    // stop ofApp.cpp comparing it to other tiles
    boiding = false;

    posTargetAchieved = true;
  }

  if (!posTargetAchieved) {
    updateBoids(frameCounter);

    // lerp dot size towards target
    dotSize = ofLerp(dotSize, dotSizeTarget, dotSizeIncrement);

    // use exponentials to control the position lerping factor
    // to counteract zeno's position
    posTargetLerp = pow(posTargetLerp, 0.99);
    pos.x = ofLerp(pos.x, posTarget.x, posTargetLerp);
    pos.y = ofLerp(pos.y, posTarget.y, posTargetLerp);
  }
  else {
    dotSize = ofLerp(dotSize, w - gutter, 0.1);
    pos.x = ofLerp(pos.x, posTarget.x, 0.1);
    pos.y = ofLerp(pos.y, posTarget.y, 0.1);
  }

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  lerpColor();
}

void Tile::drawS4() {
  ofSetColor(color);
  ofDrawCircle(pos.x, pos.y, dotSize*0.5);
}


// Scene 5
//=====================================
// morph into squares

void Tile::setupS5() {
  scene = 5;

  squircleness = 1;
  squirclenessIncrement = -0.03;
}

void Tile::updateS5(int frameCounter) {
  if (squircleness > 0) {
    squircleness += squirclenessIncrement;
  }

  // if position and size aren't what we want them to be yet
  // lerp towards them
  if (pos.distance(posTarget) > 0.2) {
    pos.x = ofLerp(pos.x, posTarget.x, 0.1);
    pos.y = ofLerp(pos.y, posTarget.y, 0.1);
  }
  if (abs(dotSize - (w - gutter)) > 0.2) {
    dotSize = ofLerp(dotSize, w - gutter, 0.1);
  }

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  lerpColor();
}

void Tile::drawS5() {
  // reset color before drawing
  ofSetColor(ofColor::white);

  ofPushMatrix();

  // translate to position minus dot size
  ofTranslate(pos.x-dotSize*0.5, pos.y-dotSize*0.5);

  ofPath squircle = pathSquircle(dotSize*0.5, squircleness);
  squircle.setFillColor(color);
  squircle.draw();

  ofPopMatrix();
}


// Scene 6
//=====================================
// fade into images

void Tile::setupS6() {
  scene = 6;

  opacity = 1;
  opacityIncrement = -0.04;
}

void Tile::updateS6(int frameCounter) {
  if (opacity > 0) {
    opacity += opacityIncrement;
  }

  // so that ofApp can update targetColor
  updateImageSometimes(frameCounter);
  // lerp color towards target color
  color.lerp(targetColor, colorLerpScalar);
}

void Tile::drawS6() {
  // reset color before drawing
  ofSetColor(ofColor::white);
  // position should now have parity with x and y
  // so just draw at exact location
  image.draw(x*w, y*h);

  // draw transparent square on top of image
  if (opacity > 0) {
    ofColor colorTrans = color;
    colorTrans.a = opacity * 255;
    ofSetColor(colorTrans);
    ofDrawRectangle(x*w, y*h, w-gutter, h-gutter);
  }
}


// Scene 7
//=====================================
// solve puzzle

void Tile::setupS7() {
  scene = 7;
}

void Tile::updateS7(int frameCounter) {
  updateS0(frameCounter);
}

void Tile::drawS7() {
  drawS0();
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
