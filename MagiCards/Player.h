#pragma once
#include <SDL.h>
#include <string>
#include "DeckLoader.h"
#include <vector>
#include "Mouse.h"

class Player
{
public:
	Player(std::string name, int deck, SDL_Renderer* renderer);

	~Player();

	std::string getName() const;

	int getDeck() const;

	bool deckLoaded() const;

	void loadDeck();

	void loadDeck(std::vector<int>& cardIDs);

	// shufles the deck and returns the shuffled card IDs
	std::vector<int> shuffleDeck(); 

	void draw(int number); // draw cards

	void drawFaceUp(int number);

	void update(Mouse* mouse);

	void render();

	int deckSize() const;

	int deckMaxSize() const;

	std::vector<Card>& hand();

private:
	std::string _name;
	int deckId_ = -1;

	//Deck deck_;
	std::vector<Card> deck_;
	std::vector<Card> hand_;
	SDL_Renderer* renderer_;
	bool loaded_ = false; //testing, remove
	int deckMaxSize_ = 0;
	int actualDeckSize;
};

