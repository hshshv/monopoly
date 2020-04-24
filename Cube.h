#ifndef Cube_h
#define Cube_h

#include "Arduino.h"
#include <Servo.h>

class Cube
{
  public:
    Cube(int servoPin);
    int Drop();
    void Point();
    int lastResulte;
  private:
    Servo pointer;
};

#endif
