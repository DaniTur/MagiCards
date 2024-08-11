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
    std::cout << "loading deck" << std::endl;
    DeckLoader::load(deckId_, deck_, renderer_);
    //loaded_ = true;//testing, remove
    //if (!deck_.cards.empty())
    if (!deck_.empty())
    {
        loaded_ = true;
    }
    deckMaxSize_ = deck_.size();
}

void Player::loadDeck(std::vector<int>& cardIDs)
{
    DeckLoader::load(cardIDs, deck_, renderer_);
    ////loaded_ = true;//testing, remove
    ////if (!deck_.cards.empty())
    if (!deck_.empty())
    {
        loaded_ = true;
    }
    deckMaxSize_ = deck_.size();
}

std::vector<int> Player::shuffleDeck()
{
    std::random_device rd;
    std::mt19937 urng(rd()); // Uniform Random Number Generator

    std::shuffle(deck_.begin(), deck_.end(), urng); // O(n)

    std::vector<int> cardIDs;
    for (Card card : deck_) 
        cardIDs.push_back(card.getId());

    return cardIDs;
}

void Player::draw(int number = 1)
{
    if (deck_.size() >= number)
    {
        for (int i = 0; i < number; i++)
        {
            hand_.push_back(deck_.back());
            deck_.pop_back();
        }
    }
    else
    {
        std::cout << "Error: trying to draw more cards than the deck has." << std::endl;
    }
}

void Player::drawFaceUp(int number = 1)
{
    if (deck_.size() >= number)
    {
        for (int i = 0; i < number; i++)
        {
            Card& card = deck_.back();
            card.turnUp();
            hand_.push_back(card);
            deck_.pop_back();
        }
    }
    else
    {
        std::cout << "Error: trying to draw more cards than the deck has." << std::endl;
    }
}

void Player::handleEvents()
{
    // the player has made a left click (left mouse up event)
    std::cout << "player handle events" << std::endl;
    // check if a card is mouse hovered and set that card to selected
    if (!hand_.empty())
    {
        bool hoveredCardFound = false;
        int i = 0;

        while (i < hand_.size() && !hoveredCardFound)
        {
            if (hand_.at(i).isMouseHovered()) {
                hoveredCardFound = true;
            }else {
                i++;
            }  

        }

        if (hoveredCardFound)  //mouse clicked on top of a card
        { 
            // TODO: arreglar la seleccion y deseleccion de cartas de la mano
            if (selectedCardIndex_ != i) // if the new selected card (i) is not the previous selected card, select it
            {
                hand_.at(i).select();
            }

            // if there was already a selected card, deselect it
            if (selectedCardIndex_ >= 0 && hand_.at(selectedCardIndex_).isSelected())
            {
                hand_.at(selectedCardIndex_).deselect();
            }

            selectedCardIndex_ = i;
        }
        else // if there is no mouse hovered card, check for selected card and deselect it
        { 
            // En el caso de jugar una carta, la carta seleccionada ya no estará en la mano y no se puede acceder a ella
            if (selectedCardIndex_ < hand_.size())
            {
                hand_.at(selectedCardIndex_).deselect();
            }
        }
    }
}

void Player::update(Mouse* mouse)
{
    // Update hand cards
    if (!hand_.empty())
    {
        for(int i = 0; i < hand_.size(); i++)
        {
            hand_.at(i).update(mouse);
        }
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

std::vector<Card>& Player::hand()
{
    return hand_;
}

int Player::handSize() const
{
    if (hand_.empty())
    {
        return 0;
    }
    return hand_.size();
}

int Player::playSelectedCard()
{
    // Tener cuidado si se copia un player porque al tener un unique_ptr puede dar problemas
    playedCard_ = std::make_unique<Card>(hand_[selectedCardIndex_]);

    hand_.erase(hand_.begin() + selectedCardIndex_);

    return selectedCardIndex_;
}

Card* Player::getPlayedCard() const
{
    // If player has played a card
    return playedCard_.get();
}

void Player::dealDamage(int damage)
{
    int d = healthPoints_ - damage;

    if (d < 0) healthPoints_ = 0;
    else healthPoints_ = d;
}

void Player::destroyActiveCard()
{
    playedCard_.reset();
}

int Player::getHealthPoints() const
{
    return healthPoints_;
}

void Player::playSelectedCard(int cardIndex)
{
    // Tener cuidado si se copia un player porque al tener un unique_ptr puede dar problemas
    playedCard_ = std::make_unique<Card>(hand_[cardIndex]);
    playedCard_->turnUp();

    hand_.erase(hand_.begin() + cardIndex);
}

bool Player::playedCardActive() const
{
    if (playedCard_.get()) return true;
    else return false;
}

void Player::renderPlayedCard(SDL_Rect destination, float proportion)
{
    if (playedCard_.get())
    {
        playedCard_->render(&destination, proportion);
    }
}
