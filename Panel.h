#ifndef PANAL_h
#define PANAL_h

#include <Arduino.h>
#include "RGBLed.h"
#define LDRPIN 30

class Panel
{
  public:
    Panel(String newName, int buyinP, int rentingP, int power1, int power2, RGBLed LED);
    Panel(String newName, int power1, int power2);
    String name;
    int buyingPrice, rentingPrice, Owner;
    int powerPin1, powerPin2;
    Static int panelsNum;
    bool WasBought();
    bool IsBuyable();
    bool IsBlocked();
    void Buy(int newOwner);
    const static int LDR = LDRPIN;
    const static Color PanelColors[7];
  private:
    RGBLed RGB;
};
#endif
