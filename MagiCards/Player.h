#pragma once
#include <SDL.h>
#include <string>
#include "DeckLoader.h"

class Player
{
public:
	Player(std::string name, int deck, SDL_Renderer* renderer);

	~Player();

	std::string getName() const;

	int getDeck() const;

	bool deckLoaded() const;

	void loadDeck(); //testing, remove

private:
	std::string _name;
	int _deckIndex = -1;

	Deck deck_;
	SDL_Renderer* renderer_;
	bool loaded_ = false; //testing, remove
};

