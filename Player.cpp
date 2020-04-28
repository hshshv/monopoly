#include "Player.h"

Player::Player(String newName, int TextBoard)
{
  name = newName;
  textBoard = TextBoard;
  ++ActivePlayers;
}

void Player::AddSkip(int skipsToAdd)
{
  skipsLeft += skipsToAdd;
}


void Player::Skip()
{
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
