#ifndef PLAYER_H
#define PLAYER_H
#include "Arduino.h"

#define initialMoney 450

class Player
{
  public:
    Player(String newName, int TextBoard);
    String name;
    int textBoard;
    int money = initialMoney;
    int location = 0;//start panel
    void AddSkip(int skipsToAdd);
    void Skip();//decrease the turns to skipe by 1
    bool Skipped();
    int SkipsLeft();
    void Eliminate();
    bool Eliminated();
    static int ActivePlayers;
  private:
    int skipsLeft = 0;
    bool isInTheGame = true;
};


#endif
