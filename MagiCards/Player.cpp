#include "Player.h"


Player::Player(std::string name, int deck, SDL_Renderer* renderer) 
    : _name(name), _deckIndex(deck), renderer_(renderer)
{
    deck_.id = deck;
}

Player::~Player()
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

bool Player::deckLoaded() const
{
    return loaded_;
}

void Player::loadDeck() 
{
    DeckLoader::load(_deckIndex, deck_, renderer_);
    //loaded_ = true;//testing, remove
    if (!deck_.cards.empty())
    {
        loaded_ = true;
    }
}
