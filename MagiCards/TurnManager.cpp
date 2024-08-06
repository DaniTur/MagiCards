#include "TurnManager.h"
#include <iostream>

TurnManager::TurnManager() : owner_(OWNER::UNDEFINED)
{
}

TurnManager::TurnManager(OWNER owner) : owner_(owner)
{
}

bool TurnManager::isPreparationTurn() const
{
	return turn_ == 0;
}

void TurnManager::next()
{
	turn_++;
}

bool TurnManager::isMyTurn() const
{
	// Server turn if turn is odd (1,3,5,...)
	// Client turn if turn is even (2, 4, 6,...)
	// Preparation turn if turn_ is zero
	if (owner_ == OWNER::SERVER && (turn_ % 2 != 0))
	{
		return true;
	}
	else if (owner_ == OWNER::CLIENT && (turn_ % 2 == 0) &&  !isPreparationTurn())
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void TurnManager::clientPreparationReady()
{
	clientPreparationTurnReady_ = true;
	std::cout << "client preparation turn ready" << std::endl;
}

void TurnManager::hostPreparationReady()
{
	hostPreparationTurnReady_ = true;
	std::cout << "host preparation turn ready" << std::endl;
}

bool TurnManager::isClientPrepared() const
{
	return clientPreparationTurnReady_;
}

bool TurnManager::isHostPrepared() const
{
	return hostPreparationTurnReady_;
}
