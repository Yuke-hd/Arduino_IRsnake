#ifndef Body_h
#define Body_h

#include "Arduino.h"

class Body
{
  public:
    Body(int x, int y);
    Body();
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    void up();
    void down();
    void left();
    void right();
   private:
    int _x;
    int _y;
};


#endif
