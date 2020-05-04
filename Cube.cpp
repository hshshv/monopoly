#include "Cube.h"

Cube::Cube(int servoPin)
{
  randomSeed(analogRead(0));
}

int Cube::Drop()
{
  randomSeed(analogRead(0));
  Serial.println("Cube::dropped");
  lastResult = OneCubeDroppingResult() + OneCubeDroppingResult();
  return (lastResult);
}

void Cube::ShowResult()
{
  //pointer.write(round((180 / 11)*lastResult - 180 / 22));
  Serial.print("Cube::");
  Serial.println(lastResult);
}

int Cube::OneCubeDroppingResult()
{
  return (1 + (analogRead(A0) % 6));
}
