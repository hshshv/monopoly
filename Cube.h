#ifndef Cube_h
#define Cube_h

#include "Arduino.h"
#include <Servo.h>

class Cube
{
  public:
    Cube(int servoPin);
    int Drop();
    void ShowResult();
    int lastResult;
  private:
    Servo pointer;
};

#endif
