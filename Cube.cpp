#include "Cube.h"

Cube::Cube(int servoPin)
{
  pointer.attach(servoPin);
  randomSeed(analogRead(0));
}

int Cube::Drop()
{
  lastResulte = random(1, 7) + random(1, 7);
  return (lastResulte);
}

void Cube::Point()
{
  pointer.write(round((180 / 11)*lastResulte - 180 / 22));
}
