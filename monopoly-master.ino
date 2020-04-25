#include <SPI.h>
#include "MFRC522.h"

#include "LQ.h"
#include <Wire.h>

#include "Button.h"
#include "Cube.h"

#include "Panel.h"
#include "Player.h"

#define SDA_PIN 48
#define RST_PIN 49

#define BuzzerPin 2

#define MaxMoney 3000
#define PlayersNum 3
#define TextBNum 3
#define PanelsNum 18

LQ LCD[TextBNum] = {LQ(0x27, 20, 4), LQ(0x27, 20, 4), LQ(0x27, 20, 4)};
Button OK(45);//change this number
Button CANCEL(46);//change this number
Cube cube(47);
Led Buzzer(BuzzerPin);
MFRC522 Chipser(SDA_PIN, RST_PIN);
int powerPins[9] = {1, 2, 3, 4, /*<GND | VCC>*/5, 6, 7, 8, 9};//digital pin 10 is LDR input pin of all panels
int RGBPins[28] = {50, 51, 52, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38};

Panel Table[PanelsNum] = {
  Panel("Start", powerPins[0], powerPins[4]), /*non buyable panel*/
  Panel("Jerusalem", 100, 20, powerPins[0], powerPins[8], RGBLed(RGBPins[8], RGBPins[9], 0)),
  Panel("Rehovot", 100, 20, powerPins[1], powerPins[4], RGBLed(RGBPins[10], RGBPins[11], 0)),
  Panel("Tel Aviv", 100, 20, powerPins[1], powerPins[5], RGBLed(RGBPins[12], RGBPins[13], 0)),
  Panel("Jail", powerPins[0], powerPins[5]),/*non buyable panel*/
  Panel("Netanya", 100, 20, powerPins[1], powerPins[6], RGBLed(RGBPins[14], RGBPins[15], 0)),
  Panel("New York", 100, 20, powerPins[1], powerPins[7], RGBLed(RGBPins[16], RGBPins[17], 0)),
  Panel("Los Angeles", 100, 20, powerPins[1], powerPins[8], RGBLed(RGBPins[18], RGBPins[19], 0)),
  Panel("London", 100, 20, powerPins[3], powerPins[5], RGBLed(RGBPins[4], RGBPins[5], 0)),
  Panel("Parking", powerPins[0], powerPins[6]),/*non buyable panel*/
  Panel("Las Vegas", 100, 20, powerPins[2], powerPins[4], RGBLed(RGBPins[20], RGBPins[21], 0)),
  Panel("Manhattan", 100, 20, powerPins[2], powerPins[5], RGBLed(RGBPins[22], RGBPins[23], 0)),
  Panel("Mexico City", 100, 20, powerPins[2], powerPins[6], RGBLed(RGBPins[24], RGBPins[25], 0)),
  Panel("Police", powerPins[0], powerPins[7]),/*non buyable panel*/
  Panel("Moscow", 100, 20, powerPins[2], powerPins[7], RGBLed(RGBPins[26], RGBPins[27], 0)),
  Panel("Stalingrad", 100, 20, powerPins[2], powerPins[8], RGBLed(RGBPins[0], RGBPins[1], 0)),
  Panel("Cairo", 100, 20, powerPins[3], powerPins[4], RGBLed(RGBPins[2], RGBPins[3], 0)),
  Panel("Paris", 100, 20, powerPins[3], powerPins[6], RGBLed(RGBPins[6], RGBPins[7], 0))
};

Player Players[PlayersNum] = {Player("Cody", 0), Player("Garrett", 1), Player("Tyler", 2)};//name is limited to 7 characters max

void setup()
{
  Player::ActivePlayers = PlayersNum;

  Players[0].addres = "CO DY JO NS";
  Players[1].addres = "GA RR ET T0";
  Players[2].addres = "TY LE R0 00";
  
  LCD[0].begin();
  LCD[1].begin();
  LCD[2].begin();

  LCD[0].backlight();
  LCD[1].backlight();
  LCD[2].backlight();

  SPI.begin();      // Initiate  SPI bus
  Chipser.PCD_Init();   // Initiate Chipser
  
  for (int i = 0; i < 9; ++i)
  {
    pinMode(powerPins[i], OUTPUT);
  }
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
  PrintOnAllBoards(Players[thisTurn].name, 0, 0, true);
  PrintOnAllBoards("s turn", Players[thisTurn].name.length(), 0, false);
  Printo("Units: ", 0, 1, thisTurn, false);
  Printo(String(Players[thisTurn].money), 7, 1, thisTurn, false);
  
  if(Players[thisTurn].Skipped())
  {
    Printo("Skips left: ", 0, 2, thisTurn, false);
    Printo(String(Players[thisTurn].SkipsLeft()), 12, 2, thisTurn, false);
    WaitForOK(0, 3);
    return;
  }
  
  Printo("Drop the cube", 0, 2, thisTurn, false);
  WaitForOK(0, 3);
  cube.Drop();
  while(Table[(Players[thisTurn].location + cube.lastResulte) % PanelsNum].Blocked())
  {
    cube.Drop();
  }
  cube.Point();
  SendPlayerTo((Players[thisTurn].location + cube.lastResulte) % PanelsNum);
  
  Printo("You arrived to", 0, 0, thisTurn, true);
  Printo(Table[Players[thisTurn].location].name, 0, 1, thisTurn, false);
  
  if (Table[Players[thisTurn].location].Buyable())
  {
    if (Table[Players[thisTurn].location].Bought())
    {
      if (Table[Players[thisTurn].location].Owner == thisTurn)
      {
        Printo("you owned it", 0, 2, thisTurn, false);
        WaitForOK(0, 3);
      }
      else
      {
        Printo("its owner is ", 0, 2, thisTurn, false);
        Printo(Players[Table[Players[thisTurn].location].Owner].name, 13, 2, thisTurn, false);
        WaitForOK(0, 3);
        PaymentRequest(thisTurn, Table[Players[thisTurn].location].Owner, Table[Players[thisTurn].location].rentingPrice);
      }
    }
    else
    {
      Printo("it is ownerless", 0, 2, thisTurn, false);
      WaitForOK(0, 3);
      if (Players[thisTurn].money >= Table[Players[thisTurn].location].buyingPrice)
      {
        Printo("it's price is: ", 0, 0, thisTurn, true);
        Printo(String(Table[Players[thisTurn].location].buyingPrice), 15, 0, thisTurn, false);
        Printo("You have: ", 0, 1, thisTurn, false);
        Printo(String(Players[thisTurn].money), 10, 1, thisTurn, false);
        Printo("Buy it?", 0, 2, thisTurn, false);
        Printo("press [OK] or [X]", 0, 3, thisTurn, false);
        if (GetYesOrNo())
        {
          PaymentRequest(thisTurn, -1, Table[Players[thisTurn].location].buyingPrice);
          Table[Players[thisTurn].location].Buy(thisTurn);
        }
      }
      else
      {
        Printo("you can't buy it now", 0, 0, thisTurn, false);
        WaitForOK(0, 3);
      }
    }
  }
  else//non-buyable panel
  {
    if (Table[Players[thisTurn].location].name == "Start")
    {
      Printo("You earned 50 units", 0, 2, thisTurn, false);
      WaitForOK(0, 3);
    }

    if (Table[Players[thisTurn].location].name == "Jail")
    {
      Printo("Enjoy your visit!", 0, 2, thisTurn, false);
      WaitForOK(0, 3);
    }

    if (Table[Players[thisTurn].location].name == "Parking")
    {
      Printo("1 skip added", 0, 2, thisTurn, false);
      Players[thisTurn].AddSkip(1);
      WaitForOK(0, 3);
    }

    if (Table[Players[thisTurn].location].name == "Police")
    {
      SendPlayerTo(4);
      Players[thisTurn].AddSkip(3);
      Printo("3 skips added", 0, 0, thisTurn, true);
    }
  }
  Printo("your turn over", 0, 2, thisTurn, true);
  WaitForOK(0, 3);
}

void SendPlayerTo(int location)
{
  Printo("Go to ", 0, 0, thisTurn, true);
  Printo(Table[location].name, 6, 0, thisTurn, true);
  while (!Table[location].Blocked()) {}
  Players[thisTurn].location = location;
}

void WaitForOK(int X, int Y)
{
  Printo("Press [OK]", X, Y, thisTurn, false);
  WaitForOK();
}

void WaitForOK()
{
  while (!OK.Activated()) {}
  delay(250);
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
  Printo("You have to pay ", 0, 0, thisTurn, true);
  Printo(String(sum), 0, 1, thisTurn, false);
  while(!ReadChips(thisTurn)){}
  Players[from].money -= sum;
  if(to > -1)
  {
    Players[to].money += sum;
  }
  Printo("Payment succeeded", 0, 2, thisTurn, false);
  Printo("Press [OK]", 0, 3, thisTurn, false);
  WaitForOK();
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


bool ReadChips(int player)
{
  if ( ! Chipser.PICC_IsNewCardPresent())
  {
    return (false);
  }
  // Select one of the cards
  if ( ! Chipser.PICC_ReadCardSerial())
  {
    return (false);
  }
  //Show UID on serial monitor
  String content = "";
  byte letter;
  for (byte i = 0; i < Chipser.uid.size; i++)
  {
    content.concat(String(Chipser.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(Chipser.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == Players[player].addres) //change here the UID of the card/cards that you want to give access
  {
    return (true);
  }

  else
  {
    return (false);
  }
}
