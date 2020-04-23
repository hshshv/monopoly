/*
   need to add all the RGB stuff, 
   jail, police, Start-panel and parking (non-buyable-panels)
   and the RFID stuff (including the PaymentRequast~ method)
*/

#include <Wire.h>
#include "LQ.h"
#include "Button.h"

#include "Panel.h"
#include "Player.h"

#define MaxMoney 3000
#define PlayersNum 3
#define TextBNum 3
#define PanelsNum 18

LQ LCD[TextBNum] = {LQ(0x27, 20, 4), LQ(0x27, 20, 4), LQ(0x27, 20, 4)};
Button OK(45);//change this number
Button CANCEL(46);//change this number

int powerPins[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

Panel Table[PanelsNum] = {
  Panel("Start", -1, 0, powerPins[0], powerPins[4]),/*non buyable panel*/
  Panel("Jail", -1, 0, powerPins[0], powerPins[5]),/*non buyable panel*/
  Panel("Parking", -1, 0, powerPins[0], powerPins[6]),/*non buyable panel*/
  Panel("Police", -1, 0, powerPins[0], powerPins[7]),/*non buyable panel*/
  Panel("Jerusalem", 100, 20, powerPins[0], powerPins[8]),
  Panel("Rehovot", 100, 20, powerPins[1], powerPins[4]),
  Panel("Tel Aviv", 100, 20, powerPins[1], powerPins[5]),
  Panel("Netanya", 100, 20, powerPins[1], powerPins[6]),
  Panel("New York", 100, 20, powerPins[1], powerPins[7]),
  Panel("Los Angeles", 100, 20, powerPins[1], powerPins[8]),
  Panel("Las Vegas", 100, 20, powerPins[2], powerPins[4]),
  Panel("Manhattan", 100, 20, powerPins[2], powerPins[5]),
  Panel("Mexico City", 100, 20, powerPins[2], powerPins[6]),
  Panel("Moscow", 100, 20, powerPins[2], powerPins[7]),
  Panel("Stalingrad", 100, 20, powerPins[2], powerPins[8]),
  Panel("Cairo", 100, 20, powerPins[3], powerPins[4]),
  Panel("London", 100, 20, powerPins[3], powerPins[5]),
  Panel("Paris", 100, 20, powerPins[3], powerPins[6])
};

Player Players[PlayersNum] = {Player("Cody", 0), Player("Garrett", 1), Player("Tyler", 2)};//name is limited to 7 characters max

void setup()
{
  Player::ActivePlayers = PlayersNum;

  LCD[0].begin();
  LCD[1].begin();
  LCD[2].begin();

  LCD[0].backlight();
  LCD[1].backlight();
  LCD[2].backlight();

  for (int i = 0; i < 9; ++i)
  {
    pinMode(powerPins[i], OUTPUT);
  }

  randomSeed(analogRead(0));//random seed for the cubes
}

int thisTurn = 0;

void loop()
{
  DoTurn();
  CheckWinning();
  ++thisTurn;
  if (thisTurn >= PlayersNum)
  {
    thisTurn = 0;
  }
}

void DoTurn()// Main function
{
  int cubesum = RollTheDice() + RollTheDice();//without double turn yet
  Players[thisTurn].location += cubesum;
  if (Players[thisTurn].location >= PanelsNum)
  {
    Players[thisTurn].location -= PanelsNum;// Potential error!!!!! check agein.
  }

  Printo("You arrived to", 0, 0, thisTurn, true);
  Printo(Table[Players[thisTurn].location].name, 0, 1, thisTurn, false);
  if (Table[Players[thisTurn].location].Buyable())
  {
    if (Table[Players[thisTurn].location].Bought())
    {
      if (Table[Players[thisTurn].location].Owner == thisTurn)
      {
        Printo("you owned it", 0, 2, thisTurn, false);
      }
      else
      {
        Printo("its owner is ", 0, 2, thisTurn, false);
        Printo(Players[Table[Players[thisTurn].location].Owner].name, 13, 2, thisTurn, false);
        Printo("You have to pay ", 0, 3, thisTurn, false);
        Printo(String(Table[Players[thisTurn].location].rentingPrice), 0, 3, thisTurn, false);
        PaymentRequest(thisTurn, Table[Players[thisTurn].location].Owner, Table[Players[thisTurn].location].rentingPrice);
        Printo("Payment succeeded", 0, 0, thisTurn, true);
        Printo("your turn over", 0, 1, thisTurn, false);
        Printo("press [OK]", 0, 3, thisTurn, false);
        WaitForOK();
      }
    }
    else
    {
      Printo("it is ownerless", 0, 2, thisTurn, false);
      if (Players[thisTurn].money >= Table[Players[thisTurn].location].buyingPrice)
      {
        Printo("press [OK]", 0, 3, thisTurn, false);
        WaitForOK();
        Printo("it's price is: ", 0, 0, thisTurn, true);
        Printo("Would you like to", 0, 1, thisTurn, false);
        Printo("buy it?", 0, 2, thisTurn, false);
        Printo("press [OK] or [X]", 0, 3, thisTurn, false);
        if (GetYesOrNo())
        {
          Printo("You have to pay ", 0, 0, thisTurn, true);
          Printo(String(Table[Players[thisTurn].location].buyingPrice), 0, 1, thisTurn, false);
          PaymentRequest(thisTurn, -1, Table[Players[thisTurn].location].buyingPrice);
          Printo("Payment succeeded", 0, 0, thisTurn, true);
          Table[Players[thisTurn].location].Buy(thisTurn);
          Printo("your turn over", 0, 1, thisTurn, false);
          Printo("press [OK]", 0, 3, thisTurn, false);
          WaitForOK();
        }
      }
      else
      {
        Printo("you can't buy it now", 0, 3, thisTurn, false);
      }
    }
  }
  else//non-buyable panel
  {

  }
}

void WaitForOK()
{
  while (!OK.Activated()) {}
}

bool GetYesOrNo()
{
  while (true)
  {
    if (OK.Activated())
    {
      return (true);
    }
    if (CANCEL.Activated())
    {
      return (false);
    }
  }
}

void PaymentRequest(int from, int to, int sum)
{
  //to (-1) is to the bank
}

long RollTheDice()
{
  return (random(1, 7)); // 1 - 6
}

void CheckWinning()
{
  int i;
  if (Players[thisTurn].money < 0)//eliminate checking
  {
    Players[thisTurn].Eliminate();
    EliminateMessege(thisTurn);
    if (Player::ActivePlayers == 1)
    {
      for (i = 0; i < PlayersNum; ++i)
      {
        if (!Players[i].Eliminated())
        {
          break;
        }
      }
      delay(5000);
      WinningMessege(i);
    }
  }

  if (Players[thisTurn].money >= MaxMoney)//winning checking
  {
    WinningMessege(thisTurn);
  }
}

void EliminateMessege(int player)
{
  PrintOnAllBoards(Players[player].name, 0, 0, true);
  PrintOnAllBoards("was eliminated.", 0, 1, false);
  PrintOnAllBoards("He will no longer", 0, 2, false);
  PrintOnAllBoards("play.", 0, 3, false);
}

void WinningMessege(int player)
{
  PrintOnAllBoards("Game ended", 0, 0, true);
  PrintOnAllBoards(Players[player].name, 0, 1, false);
  PrintOnAllBoards("is the winner", 0, 2, false);
  PrintOnAllBoards("score: ", 0, 3, false);
  PrintOnAllBoards(String(Players[player].money), 7, 3, false);
}

void PrintOnAllBoards(String Text, int X, int Y, bool Clear)
{
  for (int i = 0; i < TextBNum; ++i)
  {
    Printo(Text, X, Y, i, Clear);
  }
}

void Printo(String Text, int X, int Y, int TextBorad, bool Clear)
{
  if (Clear)
  {
    LCD[TextBorad].clear();
  }
  LCD[TextBorad].setCursor(X, Y);
  LCD[TextBorad].print(Text);
}
