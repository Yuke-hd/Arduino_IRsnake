#include <LedControl.h>
#include <binary.h>
#include <IRremote.h>
#include "Body.h"

#define maxLength 15

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

/*
  DIN connects to pin 12
  CLK connects to pin 11
  CS connects to pin 10
*/
LedControl lc = LedControl(12, 11, 10, 1);
// lc.setLed(0,row,col,true);

// Game parameters
int gameScore = 0;
int gameSpeed = 500;

enum dir {
  up,
  down,
  left,
  right
};

dir d = left;
int bodyLength = 1;
Body bodies[maxLength] ;
Body point;

byte num[10][5] = {
  {B00000111, B00000101, B00000101, B00000101, B00000111},
  {B00000001, B00000001, B00000001, B00000001, B00000001},
  {B00000111, B00000001, B00000111, B00000100, B00000111},
  {B00000111, B00000001, B00000111, B00000001, B00000111},
  {B00000101, B00000101, B00000111, B00000001, B00000001},
  {B00000111, B00000100, B00000111, B00000001, B00000111},
  {B00000111, B00000100, B00000111, B00000101, B00000111},
  {B00000111, B00000001, B00000001, B00000001, B00000001},
  {B00000111, B00000101, B00000111, B00000101, B00000111},
  {B00000111, B00000101, B00000111, B00000001, B00000111}
};

void setup() {
  //IR init
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  lc.shutdown(0, false);
  // Set brightness to a medium value
  lc.setIntensity(0, 1);
  // Clear the display
  lc.clearDisplay(0);

  for (int i = 0; i < maxLength; i++) {
    bodies[i] = Body(5, 5);
  }

  randomPoint();
  draw();
}

void(* resetFunc) (void) = 0;

void randomPoint() {
  randomSeed(analogRead(0));
  int xx = random(0, 8);
  randomSeed(analogRead(0));
  int yy = random(0, 8);
  point = Body(xx, yy);
}

void grow() {
  if (bodyLength < maxLength) {
    bodyLength++;
  }
  gameScore++;
}

void move(dir d) {
  for (int i = bodyLength; i > 0; i--) {
    bodies[i].setX(bodies[i - 1].getX());
    bodies[i].setY(bodies[i - 1].getY());
  }
  switch (d) {
    case up:
      bodies[0].up();
      break;
    case down:
      bodies[0].down();
      break;
    case left:
      bodies[0].left();
      break;
    case right:
      bodies[0].right();
      break;
  }

  for (int i = bodyLength - 1; i > 0; i--) {
    if (bodies[0].getX() == bodies[i].getX() && bodies[0].getY() == bodies[i].getY()) {
      drawScore();
      delay(5000);
      resetFunc();
    }
  }

  if (bodies[0].getX() == point.getX() && bodies[0].getY() == point.getY()) {
    grow();
    randomPoint();
  }
}


void draw() {
  lc.clearDisplay(0);
  lc.setLed(0, point.getY(), point.getX(), true);
  for (int i = 0; i < bodyLength; i++) {
    lc.setLed(0, bodies[i].getY(), bodies[i].getX(), true);
  }
}

void IRreceive() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch (results.value) {
      case 0x2A99: //Keypad button "5"
        d = up;
        break;
      case 0x4A99:
        d = down;
        break;
      case 0xA99:
        d = left;
        break;
      case 0x1499:
        d = right;
        break;
      case 0x2499:
        if (gameSpeed >= 50) {
          gameSpeed = gameSpeed - 50;
        }
        break;
      case 0x6499:
        if (gameSpeed < 800) {
          gameSpeed = gameSpeed + 50;
        }
        break;

    }
    irrecv.resume();
  }
}

void drawScore() {
  lc.clearDisplay(0);
  int m;
  int n;
  if (gameScore >= 10) {
    m = gameScore / 10;
    n = gameScore - m * 10;
  } else {
    m = 0;
    n = gameScore - 1;
  }
  Serial.println(m);
  Serial.println(n);
  byte s[5];
  for (int k = 0; k < 5; k++) {
    s[k] = (num[m][k] << 4) + num[n][k];
  }
  lc.setRow(0, 2, s[0]);
  lc.setRow(0, 3, s[1]);
  lc.setRow(0, 4, s[2]);
  lc.setRow(0, 5, s[3]);
  lc.setRow(0, 6, s[4]);
}

void loop() {
  IRreceive();
  move(d);
  draw();

  delay(gameSpeed);
}
