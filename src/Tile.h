#pragma once
#include "ofMain.h"

class Tile {

public:

  void move(ofVec2f dir, bool easing);

  void update(int frameCounter, float secondsElapsed);
  void updateS0(int frameCounter);

  void draw();
  void drawS0();
  void drawS1();

  ofImage image;
  bool moving = false;
  bool updateImage;
  int initialX, initialY;
  int curFrame;
  int scene;
  float* transitions;
  int tileMoveAnimFrames;
  int tileSizeAnimFrames;
  float startX, startY, endX, endY;
  float x, y;
  float w, h, gutter;
  float dotSize;

  float easeOutCubic(float t, float b, float c, float d);
  float easeInCubic(float t, float b, float c, float d);

  Tile(int _x, int _y, float _w, float _h, float _gutter);

private:

};
