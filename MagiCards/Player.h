#pragma once
#include <SDL.h>
#include <string>
#include "DeckLoader.h"
#include <vector>

class Player
{
public:
	Player(std::string name, int deck, SDL_Renderer* renderer);

	~Player();

	std::string getName() const;

	int getDeck() const;

	bool deckLoaded() const;

	void loadDeck();

	void shuffleDeck();

	void draw(int number);

	void render();

	int deckSize() const;

	int deckMaxSize() const;

	std::vector<Card> hand(); //returns a copy of the hand

private:
	std::string _name;
	int deckId_ = -1;

	//Deck deck_;
	std::vector<Card> deck_;
	std::vector<Card> hand_;
	SDL_Renderer* renderer_;
	bool loaded_ = false; //testing, remove
	int deckMaxSize_ = -1;
};

