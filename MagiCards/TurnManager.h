#pragma once

#include "Common.h"

class TurnManager
{
public:

	TurnManager();

	TurnManager(OWNER owner);

	bool isPreparationTurn() const;

	void next();

	bool isMyTurn() const;

	void clientPreparationReady();

	void hostPreparationReady();

	bool isClientPrepared() const;

	bool isHostPrepared() const;

private:

	// Describes if the class belongs to a client or server instance
	OWNER owner_;

	int turn_ = 0;

	bool playerReady = false;

	bool opponentPlayerReady = false;

	bool clientPreparationTurnReady_ = false;
	bool hostPreparationTurnReady_ = false;
};

