#include "Player.h"
#include <random>

Player::Player(std::string name, int deck, SDL_Renderer* renderer) 
    : _name(name), deckId_(deck), renderer_(renderer)
{
    //deck_.id = deck;
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
    return deckId_;
}

bool Player::deckLoaded() const
{
    return loaded_;
}

void Player::loadDeck() 
{
    DeckLoader::load(deckId_, deck_, renderer_);
    //loaded_ = true;//testing, remove
    //if (!deck_.cards.empty())
    if (!deck_.empty())
    {
        loaded_ = true;
    }
    deckMaxSize_ = deck_.size();
}

void Player::shuffleDeck()
{
    std::random_device rd;
    std::mt19937 urng(rd()); // Uniform Random Number Generator
    //std::shuffle(deck_.cards.begin(), deck_.cards.end(), urng); // O(n)

    std::shuffle(deck_.begin(), deck_.end(), urng); // O(n)

    std::cout << "Deck shuffled, cards id are:" << std::endl;
    for (Card card : deck_)
    {
        std::cout << card.getId() << ",";
    }
    std::cout << std::endl;
}

void Player::draw(int number = 1)
{
    //if (deck_.cards.size() >= number)
    if (deck_.size() >= number)
    {
        for (int i = 0; i < number; i++)
        {
            //hand_.push_back(deck_.cards.back());
            hand_.push_back(deck_.back());
            deck_.pop_back();
        }
        std::cout << "the actual hand is:" << std::endl;
        for (Card c : hand_)
        {
            std::cout << c.getId() << ", ";
        }
        std::cout << std::endl;
        std::cout << "deck size: " << deck_.size() << std::endl;
    }
    else
    {
        std::cout << "Error: trying to draw more cards than the deck has." << std::endl;
    }
}

void Player::render()
{
    // Render hand cards 
     
    // Render deck
    SDL_Texture* background = IMG_LoadTexture(renderer_, IMG_DECK);
    SDL_Rect src, dst;
    src.x = 0;
    src.y = 0;
    src.w = 225;
    src.h = 310;

    dst.x = 1280 - 225 - 25;
    dst.y = 720 - 310 - 25;
    dst.w = 225 * (0.70);
    dst.h = 310 * (0.70);
    SDL_RenderCopy(renderer_, background, &src, &dst);
}

int Player::deckSize() const
{
    return deck_.size();
}

int Player::deckMaxSize() const
{
    return deckMaxSize_;
}

std::vector<Card> Player::hand()
{
    return hand_;
}
