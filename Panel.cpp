#include "Panel.h"

Panel::Panel(String newName, int buyingP, int rentingP, int power1, int power2, RGBLed NewRGB)
{
  name = newName;
  buyingPrice = buyingP;
  rentingPrice = rentingP;
  powerPin1 = power1;
  powerPin2 = power2;
  Owner = -1;
  RGB = NewRGB;
}


Panel::Panel(String newName, int power1, int power2):RGB(0,0,0)
{
  name = newName;
  buyingPrice = 0;
  rentingPrice = 0;
  powerPin1 = power1;
  powerPin2 = power2;
  Owner = -1;
}

bool Panel::WasBought()
{
  return (Owner > -1);
}

bool Panel::IsBlocked()
{
  digitalWrite(powerPin1, HIGH);
  digitalWrite(powerPin2, HIGH);
  bool IsBlocked = digitalRead(LDR);
  digitalWrite(powerPin1, LOW);
  digitalWrite(powerPin2, LOW);
  return (IsBlocked);
}

void Panel::Buy(int newOwner)
{
  if (!WasBought() && buyingPrice > 0)
  {
    Owner = newOwner;
  }
  RGB.TurnOn(PanelColors[Owner]);
}

bool Panel::IsBuyable()
{
  return(buyingPrice > 0);
}

const static Color Panel::PanelColors[7] = {colors::Red, colors::Green, colors::Yellow, colors::Blue, colors::Purple, colors::Lightblue, colors::White};
