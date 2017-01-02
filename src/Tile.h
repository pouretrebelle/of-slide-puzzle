#pragma once
#include "ofMain.h"

class Tile {

public:

  void move(ofVec2f dir, bool easing);

  void setupS2();

  void update(int frameCounter, float secondsElapsed);
  void updateS0(int frameCounter);
  void updateS1(int frameCounter);
  void updateS2(int frameCounter);

  void draw();
  void drawS0();
  void drawS1();
  void drawS2();

  ofImage image;
  bool moving = false;
  bool updateImage;
  int initialX, initialY;
  int curFrame;
  int scene;
  float* transitions;
  float startX, startY, endX, endY;
  float x, y;
  float w, h, gutter;

  // Scene 0
  int tileMoveAnimFrames;

  // Scene 1
  int tileSizeAnimFrames;
  float dotSize;

  // Scene 2
  ofVec2f pos;
  ofVec2f vel;
  ofColor color;

  float easeOutCubic(float t, float b, float c, float d);
  float easeInCubic(float t, float b, float c, float d);

  Tile(int _x, int _y, float _w, float _h, float _gutter);

private:

};
