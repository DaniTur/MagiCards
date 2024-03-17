#pragma once
#include <string>
class Player
{
public:
	Player();
	Player(std::string name);
	~Player();

private:
	std::string _name;
	int _deckIndex;
	//Deck deck;
};

