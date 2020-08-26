#include "Arduino.h"
#include "Body.h"

Body::Body(int x, int y) // the constructor at work
{
  _x = x;
  _y = y;
}
Body::Body(){
  
}
int Body::getX() {
  return _x;
}
void Body::setX(int x){
  _x = x;
}
int Body::getY() {
  return _y;
}
void Body::setY(int y){
  _y = y;
}

void Body::up() {
  if (_y == 0) {
    _y = 7;
  } else {
    --_y;
  }
}

void Body::down() {
  if (_y == 7) {
    _y = 0;
  } else {
    ++_y;
  }
}

void Body::left() {
  if (_x == 0) {
    _x = 7;
  } else {
    --_x;
  }
}

void Body::right() {
  if (_x == 7) {
    _x = 0;
  } else {
    ++_x;
  }
}
