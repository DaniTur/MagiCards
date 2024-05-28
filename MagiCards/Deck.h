#pragma once
#include "Card.h"
#include <vector>

struct Deck
{
	uint8_t id;
	std::vector<Card> cards;
};