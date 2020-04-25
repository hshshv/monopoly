#include "Cube.h"

Cube::Cube(int servoPin)
{
  pointer.attach(servoPin);
  randomSeed(analogRead(0));
}

int Cube::Drop()
{
  lastResult = random(1, 7) + random(1, 7);
  return (lastResult);
}

void Cube::ShowResult()
{
  pointer.write(round((180 / 11)*lastResult - 180 / 22));
}
