#ifndef PANAL_h
#define PANAL_h
#include<Arduino.h>
#define LDRPIN 30

class Panel
{
  public:
    Panel(String newName, int buyinP, int rentingP, int power1, int power2);
    String name;
    int buyingPrice, rentingPrice, Owner;
    int powerPin1, powerPin2;
    bool Bought();
    bool Buyable();
    bool Blocked();
    void Buy(int newOwner);
    const static int LDR = LDRPIN;
};
#endif
