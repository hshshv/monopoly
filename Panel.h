#ifndef PANAL_h
#define PANAL_h

#include <Arduino.h>
#define LDRPIN 30

class Panel
{
  public:
    Panel(String newName, int buyinP, int rentingP);
    Panel(String newName);
    String name;
    int buyingPrice, rentingPrice, Owner;
    static int panelsNum;
    bool WasBought();
    bool IsBuyable();
    bool IsBlocked();
    void Buy(int newOwner);
    bool ImBlocked;
};
#endif
