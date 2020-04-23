#include "Panel.h"

Panel::Panel(String newName, int buyingP, int rentingP, int power1, int power2)
{
  name = newName;
  buyingPrice = buyingP;
  rentingPrice = rentingP;
  powerPin1 = power1;
  powerPin2 = power2;
  Owner = -1;
}

bool Panel::Bought()
{
  return (Owner > -1);
}

bool Panel::Blocked()
{
  digitalWrite(powerPin1, HIGH);
  digitalWrite(powerPin2, HIGH);
  bool blocked = digitalRead(LDR);
  digitalWrite(powerPin1, LOW);
  digitalWrite(powerPin2, LOW);
  return (blocked);
}

void Panel::Buy(int newOwner)
{
  if (!Bought())
  {
    Owner = newOwner;
  }
}

bool Panel::Buyable()
{
  return(buyingPrice > 0);
}
