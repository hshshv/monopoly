/*
   computer version
   !!!!!!!!!!!!!!!!!!

*/
#include "Cube.h"
#include "Panel.h"
#include "Player.h"
#define JailPanel 4
#define MaxMoney 3000

Cube cube(47);

void setup()
{
  Serial.begin(9600);
  Serial.println("Monopoly (c) yedidya");
  delay(3000);
  Serial.println("Starting game");
}


Player Players[] = {Player("Moty", 0), Player("Arick", 1),/* Player("Tyler", 2)*/};//name is limited to 7 characters max

Panel Table[] = {
  Panel("Start"), /*non IsBuyable panel*/
  Panel("Jerusalem", 50, 5),
  Panel("Rehovot", 55, 7),
  Panel("Tel Aviv", 60, 9),
  Panel("Jail"),/*non IsBuyable panel*/
  Panel("Netanya", 65, 11),
  Panel("New York", 70, 13),
  Panel("Los Angeles", 75, 18),
  Panel("London", 80, 20),
  Panel("Parking"),/*non IsBuyable panel*/
  Panel("Las Vegas", 85, 23),
  Panel("Manhattan", 90, 26),
  Panel("Mexico City", 95, 30),
  Panel("Police"),/*non IsBuyable panel*/
  Panel("Moscow", 100, 33),
  Panel("Stalingrad", 105, 37),
  Panel("Cairo", 110, 40),
  Panel("Paris", 115, 44)
};

int thisTurn = 0;

void loop()
{
  DoTurn();
  CheckWinning();
  ++thisTurn;
  if (thisTurn >= Player::ActivePlayers)
  {
    thisTurn = 0;
  }
}

void DoTurn()// Main function
{
  Serial.println("-----");
  Serial.print(Players[thisTurn].name);
  Serial.println("s turn");
  Serial.print("You have: ");
  Serial.print(Players[thisTurn].money);
  Serial.println(" Units");
  
  if (Players[thisTurn].Skipped())
  {
    Players[thisTurn].Skip();
    Serial.print("Skips left: ");
    Serial.println(String(Players[thisTurn].SkipsLeft()));
    WaitForOK(0, 3);
    return;
  }

  SmartPrint("Drop the cube", 0, 2, thisTurn, false);
  WaitForOK(0, 3);
  cube.Drop();
  while (Table[(Players[thisTurn].location + cube.lastResult) % Panel::panelsNum].IsBlocked())
  {
    cube.Drop();
  }
  cube.ShowResult();
  int NewLocation = Players[thisTurn].location + cube.lastResult;
  if(NewLocation >= Panel::panelsNum)
  {
    Players[thisTurn].money += 50;
    Serial.println("you earned 50 Units");
    NewLocation = NewLocation % Panel::panelsNum;
  }
  SendPlayerTo(NewLocation);

  SmartPrint("You arrived to", 0, 0, thisTurn, true);
  SmartPrint(Table[Players[thisTurn].location].name, 0, 1, thisTurn, false);

  if (Table[Players[thisTurn].location].IsBuyable())
  {
    if (Table[Players[thisTurn].location].WasBought())
    {
      if (Table[Players[thisTurn].location].Owner == thisTurn)
      {
        SmartPrint("you owned it", 0, 2, thisTurn, false);
        WaitForOK(0, 3);
      }
      else
      {
        SmartPrint("its owner is ", 0, 2, thisTurn, false);
        SmartPrint(Players[Table[Players[thisTurn].location].Owner].name, 13, 2, thisTurn, false);
        WaitForOK(0, 3);
        PaymentRequest(thisTurn, Table[Players[thisTurn].location].Owner, Table[Players[thisTurn].location].rentingPrice);
      }
    }
    else
    {
      SmartPrint("it is ownerless", 0, 2, thisTurn, false);
      WaitForOK(0, 3);
      if (Players[thisTurn].money >= Table[Players[thisTurn].location].buyingPrice)
      {
        SmartPrint("it's price is: ", 0, 0, thisTurn, true);
        SmartPrint(String(Table[Players[thisTurn].location].buyingPrice), 15, 0, thisTurn, false);
        Serial.print("its renting Price is: ");
        Serial.println(Table[Players[thisTurn].location].rentingPrice);
        SmartPrint("You have: ", 0, 1, thisTurn, false);
        SmartPrint(String(Players[thisTurn].money), 10, 1, thisTurn, false);
        SmartPrint("Buy it?", 0, 2, thisTurn, false);
        SmartPrint("press [OK] or [X]", 0, 3, thisTurn, false);
        if (GetYesOrNo())
        {
          PaymentRequest(thisTurn, -1, Table[Players[thisTurn].location].buyingPrice);
          Table[Players[thisTurn].location].Buy(thisTurn);
        }
      }
      else
      {
        SmartPrint("you can't buy it now", 0, 0, thisTurn, false);
        WaitForOK(0, 3);
      }
    }
  }
  else//non-IsBuyable panel
  {
    if (Table[Players[thisTurn].location].name == "Start")
    {
      SmartPrint("You earned 50 units", 0, 2, thisTurn, false);
      WaitForOK(0, 3);
    }

    if (Table[Players[thisTurn].location].name == "Jail")
    {
      SmartPrint("Enjoy your visit!", 0, 2, thisTurn, false);
      WaitForOK(0, 3);
    }

    if (Table[Players[thisTurn].location].name == "Parking")
    {
      SmartPrint("1 skip added", 0, 2, thisTurn, false);
      Players[thisTurn].AddSkip(1);
      WaitForOK(0, 3);
    }

    if (Table[Players[thisTurn].location].name == "Police")
    {
      SendPlayerTo(JailPanel);
      Players[thisTurn].AddSkip(3);
      SmartPrint("3 skips added", 0, 0, thisTurn, true);
    }
  }
  SmartPrint("your turn over", 0, 2, thisTurn, true);
  WaitForOK(0, 3);
}

void SendPlayerTo(int location)
{
  SmartPrint("Go to ", 0, 0, thisTurn, true);
  SmartPrint(Table[location].name, 6, 0, thisTurn, true);
  Table[Players[thisTurn].location].ImBlocked = false;
  Table[location].ImBlocked = true;
  Players[thisTurn].location = location;
}

void WaitForOK(int X, int Y)
{
  SmartPrint("Press [OK]", X, Y, thisTurn, false);
  WaitForOK();
}

void WaitForOK()
{
  Serial.println("enter OK");
  while(!Serial.available());
  Serial.readString();
}

bool GetYesOrNo()
{
  Serial.println("yes \ no  ?");
  while (!Serial.available()) {}
  String answer = Serial.readString();
  return (answer[0] == 'y');
}

void PaymentRequest(int from, int to, int sum)
{
  //to (-1) is to the bank
  SmartPrint("You have to pay ", 0, 0, thisTurn, true);
  SmartPrint(String(sum), 0, 1, thisTurn, false);
  while (!ReadChips(thisTurn)) {}
  Players[from].money -= sum;
  if (to > -1)
  {
    Players[to].money += sum;
  }
  SmartPrint("Payment succeeded", 0, 2, thisTurn, false);
  SmartPrint("Press [OK]", 0, 3, thisTurn, false);
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
      for (i = 0; i < Player::ActivePlayers; ++i)
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
  PrintnOnAllBoards(Players[player].name, 0, 0, true);
  PrintnOnAllBoards("was eliminated.", 0, 1, false);
  PrintnOnAllBoards("He will no longer play", 0, 2, false);
  //PrintnOnAllBoards("play.", 0, 3, false);
}

void WinningMessege(int player)
{
  PrintnOnAllBoards("Game ended", 0, 0, true);
  PrintnOnAllBoards(Players[player].name, 0, 1, false);
  PrintnOnAllBoards("is the winner", 0, 2, false);
  PrintnOnAllBoards("score: ", 0, 3, false);
  PrintnOnAllBoards(String(Players[player].money), 7, 3, false);
  while(true){}
}

void PrintnOnAllBoards(String Text, int X, int Y, bool Clear)
{
  //Serial.println("everyone listen: ");
  SmartPrint(Text, X, Y, 0, Clear);
}

void SmartPrint(String Text, int X, int Y, int TextBorad, bool Clear)
{
  //Serial.print(Players[TextBorad].name);
  //Serial.print(": ");
  Serial.println(Text);
}

bool ReadChips(int numberofsomthingthatdoesntmatter)
{
  Serial.println("*Reading CHIPSSS*");
  return (true);
}
