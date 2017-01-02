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


  // Shared Functions
  //===================================

  void updateImageSometimes(int frameCounter);
  void updateBoids(int frameCounter);
  void drawSquircle(float radius, float t);


  // Scene 0
  //===================================
  // solve puzzle

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
  // reduce to dots

  int tileSizeAnimFrames;
  float dotSize;
  void drawS1();
  void updateS1(int frameCounter);


  // Scene 2
  //===================================
  // boid about

  ofVec2f pos;
  ofVec2f vel;
  ofColor color;
  ofColor targetColor;
  float colorLerpScalar;
  float speed;
  float speedMax;
  float speedMaxIncrement;
  float speedMaxLimit;
  float avoidanceDist;
  float avoidanceScalar;
  float cohesionDist;
  float cohesionScalar;
  float alignDist;
  float alignScalar;
  void setupS2();
  void updateS2(int frameCounter);
  void drawS2();


  // Scene 3
  //===================================
  // grow into circles

  float squircleness;
  float squirclenessIncrement;
  float dotSizeTarget;
  float dotSizeIncrement;
  float avoidanceDistIncrement;
  void setupS3();
  void updateS3(int frameCounter);
  void drawS3();


  // Helper Functions
  //===================================

  float easeOutCubic(float t, float b, float c, float d);
  float easeInCubic(float t, float b, float c, float d);

private:

};
