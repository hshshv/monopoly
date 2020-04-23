#include "Button.h"

Button::Button(byte pin)
{
  InPin = pin;
  pinMode(InPin, INPUT);
  digitalWrite(InPin, HIGH);
}

bool Button::Activated()
{
  return (digitalRead(InPin) == LOW);
}
