#include "Player.h"

Player::Player()
{
}

Player::Player(std::string name, int deck) : _name(name), _deckIndex(deck)
{
}

std::string Player::getName()
{
    return _name;
}

int Player::getDeck()
{
    return _deckIndex;
}
