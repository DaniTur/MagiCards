#include "Player.h"

Player::Player()
{
}

Player::Player(std::string name, int deck) : _name(name), _deckIndex(deck)
{
}

std::string Player::getName() const
{
    return _name;
}

int Player::getDeck() const
{
    return _deckIndex;
}
