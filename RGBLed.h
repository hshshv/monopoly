#ifndef RGBLED_H
#define RGBLED_H

#include "Led.h"
#include "Arduino.h"
#include "colors.h"


class RGBLed
{
  private:
    Led Red;
    Led Green;
    Led Blue;
    bool IsOn;
    Color _colors[colors::size] = { colors::Red, colors::Green, colors::Blue, colors::Purple, colors::Lightblue, colors::Yellow, colors::White };
    int ThisColor;
    int MyClarity = 0;
  public:
    RGBLed(int RedPin, int GreenPin, int BluePin);
    RGBLed();
    void SetPins(int RedPin, int GreenPin, int BluePin);
    bool On();
    void Change();
    void SetColor(int rednum, int greenum, int bluenum);
    void TurnOn();
    void TurnOn(Color NewColor);
    void TurnOn(int newclarity);
    void TurnOn(Color NewColor, int newclarity);
    void TurnOff();
    void Next(int Step);
    void Next();
    void Blip(int Time);
    Color GetColor();
    int Clarity();
};
#endif
