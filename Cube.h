#ifndef Cube_h
#define Cube_h

#include "Arduino.h"


class Cube
{
  public:
    Cube(int servoPin);
    int Drop();
    static int OneCubeDroppingResult();
    void ShowResult();
    int lastResult;
};

#endif
