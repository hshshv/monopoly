#include "RGBLed.h"

RGBLed::RGBLed(int RedPin, int GreenPin, int BluePin) : Red(RedPin), Green(GreenPin), Blue(BluePin)
{
  SetPins(RedPin, GreenPin, BluePin);
}

RGBLed::RGBLed()
{
  
}

void RGBLed::SetPins(int RedPin, int GreenPin, int BluePin)
{
  Red.SetPin(RedPin);
  Green.SetPin(GreenPin);
  Blue.SetPin(BluePin);
  TurnOff();
  ThisColor = colors::White;
}

bool RGBLed::On()
{
  return (IsOn);
}

void RGBLed::Change()
{
  IsOn = !IsOn;
  if (IsOn)
  {
    TurnOn();
    return;
  }
  TurnOff();
}

void RGBLed::SetColor(int rednum, int greenum, int bluenum)
{
  if (rednum > 0) {
    Red.TurnOn(rednum);
  }
  if (greenum > 0) {
    Green.TurnOn(greenum);
  }
  if (bluenum > 0) {
    Blue.TurnOn(bluenum);
  }
  MyClarity = round((bluenum + greenum + rednum) / 3);
}

int RGBLed::Clarity()
{
  return (MyClarity);
}

void RGBLed::TurnOff()
{
  Red.TurnOff();
  Green.TurnOff();
  Blue.TurnOff();
  IsOn = false;
  MyClarity = 0;
}

void RGBLed::TurnOn(Color NewColor, int newclarity)
{
  TurnOff();
  switch (NewColor)
  {
    case colors::Red: SetColor(newclarity, 0, 0); ThisColor = 0; break;
    case colors::Green: SetColor(0, newclarity, 0); ThisColor = 1; break;
    case colors::Blue: SetColor(0, 0, newclarity); ThisColor = 2; break;
    case colors::Purple: SetColor(round(newclarity / 2), 0, round(newclarity / 2)); ThisColor = 3; break;
    case colors::Lightblue: SetColor(0, round(newclarity / 2), round(newclarity / 2)); ThisColor = 4; break;
    case colors::Yellow: SetColor(round(newclarity / 2), round(newclarity / 2), 0); ThisColor = 5; break;
    case colors::White: SetColor(round(newclarity / 3), round(newclarity / 3), round(newclarity / 3)); ThisColor = 6; break;
      break;
  }
  IsOn = true;
}

void RGBLed::TurnOn(int newclarity)
{
  TurnOn(_colors[ThisColor], newclarity);
}

void RGBLed::TurnOn(Color NewColor)
{
  if (MyClarity == 0) {
    MyClarity = 100;
  }
  TurnOn(NewColor, MyClarity);
}

void RGBLed::TurnOn()
{
  TurnOn(_colors[ThisColor], MyClarity);
}

void RGBLed::Next(int Step)
{
  ThisColor += Step;
  while (ThisColor > colors::size - 1)
  {
    ThisColor -= colors::size;
  }
  TurnOn(_colors[ThisColor]);
}

void RGBLed::Next()
{
  Next(1);
}

Color RGBLed::GetColor()
{
  return (_colors[ThisColor]);
}

void RGBLed::Blip(int Time)
{
  Change();
  delay(Time);
  Change();
}
