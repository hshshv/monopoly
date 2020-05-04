#include "Panel.h"

Panel::Panel(String newName, int buyingP, int rentingP)
{
  name = newName;
  buyingPrice = buyingP;
  rentingPrice = rentingP;
  Owner = -1;
  ++panelsNum;
  Serial.print("Panel::"); Serial.print(name); Serial.print(" ("); Serial.print(panelsNum); Serial.println(")");
}


Panel::Panel(String newName)
{
  name = newName;
  buyingPrice = 0;
  rentingPrice = 0;
  Owner = -1;
  ++panelsNum;
  Serial.print("Panel::"); Serial.print(name); Serial.print(" ("); Serial.print(panelsNum); Serial.println(")");
}

bool Panel::WasBought()
{
  return (Owner > -1);
}

bool Panel::IsBlocked()
{
  return (ImBlocked);
}

void Panel::Buy(int newOwner)
{
  if (!WasBought() && buyingPrice > 0)
  {
    Owner = newOwner;
  }
  Serial.print(name);
  Serial.println("::has just been brought");
}

bool Panel::IsBuyable()
{
  return(buyingPrice > 0);
}

static int Panel::panelsNum = 0;
