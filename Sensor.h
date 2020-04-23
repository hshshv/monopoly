#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

class Sensor
{
  public:
    byte InPin;
    /*
      Sensor(byte pin);
    */
    bool Activated();
};
#endif
