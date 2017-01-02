#pragma once
#include "ofMain.h"

class Tile {

public:

  Tile(int _x, int _y, float _w, float _h, float _gutter);
  void update(int frameCounter, float secondsElapsed);
  void draw();

  int curFrame;
  int scene;
  float* transitions;


  // Scene 0
  //===================================

  ofImage image;
  bool moving = false;
  bool updateImage;
  int initialX, initialY;
  float w, h, gutter;
  float x, y;
  float startX, startY, endX, endY;
  int tileMoveAnimFrames;
  void move(ofVec2f dir, bool easing);
  void updateS0(int frameCounter);
  void drawS0();


  // Scene 1
  //===================================

  int tileSizeAnimFrames;
  float dotSize;
  void drawS1();
  void updateS1(int frameCounter);


  // Scene 2
  //===================================

  ofVec2f pos;
  ofVec2f vel;
  ofColor color;
  float speed;
  void setupS2();
  void updateS2(int frameCounter);
  void drawS2();


  // Helper Functions
  //===================================

  float easeOutCubic(float t, float b, float c, float d);
  float easeInCubic(float t, float b, float c, float d);

private:

};
