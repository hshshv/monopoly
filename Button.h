#ifndef Button_h
#define Button_h

#include "Sensor.h"
#include "Arduino.h"

class Button: public Sensor
{
  public:
    Button(byte pin);
    bool Activated();
  private:
    //byte InPin;
};
#endif
