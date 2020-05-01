#include "Player.h"

Player::Player(String newName, int TextBoard)
{
  name = newName;
  ++ActivePlayers;
  Serial.print(name); Serial.println("joined the game!");
}

void Player::AddSkip(int skipsToAdd)
{
  skipsLeft += skipsToAdd;
}


void Player::Skip()
{
  Serial.println("you have been skipped");
  --skipsLeft;
}

bool Player::Skipped()
{
  return (skipsLeft > 0 || Eliminated());
}

int Player::SkipsLeft()
{
  return (skipsLeft);
}

bool Player::Eliminated()
{
  return (!isInTheGame);
}

void Player::Eliminate()
{
  isInTheGame = false;
  money = 0;
  location = -1;
  --ActivePlayers;
}

static int Player::ActivePlayers;
