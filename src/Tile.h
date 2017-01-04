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

  void updateImageSometimes(int frameCounter, int often);
  void updateBoids(int frameCounter);
  ofPath pathSquircle(float radius, float t);


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
  void setupS0();
  void updateS0(int frameCounter, float secondsElapsed);
  void drawS0();


  // Scene 1
  //===================================
  // reduce to dots

  int tileSizeAnimFrames;
  float dotSize;
  void setupS1();
  void updateS1(int frameCounter);
  void drawS1();


  // Scene 2
  //===================================
  // boid about

  ofVec2f pos;
  ofVec2f vel;
  ofColor color;
  ofColor targetColor;
  bool boiding;
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
  ofPath squircle;
  void setupS3();
  void updateS3(int frameCounter);
  void drawS3();


  // Scene 4
  //===================================
  // reform grid

  ofVec2f posTarget;
  bool posTargetAchieved;
  float posTargetLerp;
  float posTargetIncrement;
  void setupS4();
  void updateS4(int frameCounter);
  void drawS4();


  // Scene 5
  //===================================
  // morph into squares

  void setupS5();
  void updateS5(int frameCounter);
  void drawS5();


  // Scene 6
  //===================================
  // fade into images

  void setupS6();
  void updateS6(int frameCounter);
  void drawS6();


  // Scene 7
  //===================================
  // solve puzzle

  float opacity;
  float opacityIncrement;
  void setupS7();
  void updateS7(int frameCounter);
  void drawS7();


  // Helper Functions
  //===================================

  float easeOutCubic(float t, float b, float c, float d);
  float easeInCubic(float t, float b, float c, float d);

private:

};
