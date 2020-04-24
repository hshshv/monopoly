#include "Led.h"

Led:: Led (int pin)
{
  SetPin(pin);
}

Led:: Led ()
{
 Led(13);
}

void Led::SetPin(int pin)
{
  Pin = pin;
  pinMode(Pin, OUTPUT);
  TurnOff();
}
bool Led:: On()
{
  return (IsOn);
}

void Led:: TurnOn()
{
  digitalWrite(Pin, HIGH);
  IsOn = true;
  MyClarity = 50;
}

void Led::TurnOn(int clarity)
{
  if(!(clarity > 0 && clarity < 101))
  {
    clarity = 50;
  }
  else if(clarity == 0)
  {
    TurnOff();
  }
  analogWrite(Pin, map(clarity, 0 , 100, 87, 255));
  IsOn = true;
  MyClarity = clarity;
}

void Led:: TurnOff()
{
  digitalWrite(Pin, LOW);
  IsOn = false;
  MyClarity = 0;
}

int Led::Clarity()
{
  return(MyClarity);
}

void Led:: Change()
{
  IsOn = !IsOn;
  if (IsOn)
  {
    TurnOn();
    return;
  }
  TurnOff();
}

void Led::Blip(int Time)
{
  Change();
  delay(Time);
  Change();
}
