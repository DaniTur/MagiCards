#pragma once
#include <string>
class Player
{
public:
	Player();
	Player(std::string name, int deck);
	~Player();

	std::string getName() const;

	int getDeck() const;

private:
	std::string _name;
	int _deckIndex;
	//Deck deck;
};

