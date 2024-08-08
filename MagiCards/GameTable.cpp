#include "GameTable.h"
#include <SDL_image.h>
#include "ResourcesList.h"


GameTable::GameTable(SDL_Renderer* renderer, Player* player, Player* opponent, OWNER owner)
	: renderer_(renderer), player_(player), playerOpponent_(opponent), owner_(owner), turnManager_(owner)
{
	//background_ = IMG_LoadTexture(renderer_, IMG_GAME_TABLE);
	background_ = std::unique_ptr<SDL_Texture, TextureDestructor>(IMG_LoadTexture(renderer_, IMG_GAME_TABLE), SDL_DestroyTexture);

	sRect_.x = 0;
	sRect_.y = 0;
	sRect_.w = RESOLUTION_WIDTH;
	sRect_.h = RESOLUTION_HEIGHT;

	dRect_.x = 0;
	dRect_.y = 0;
	dRect_.w = RESOLUTION_WIDTH;
	dRect_.h = RESOLUTION_HEIGHT;

	textFont_ = TTF_OpenFont(TEXT_FONT, 14);

	SDL_Rect dst;
	// 25 right marging deck to screen, 115 deck aprox, 20 maring to deck
	dst.w = 70;
	dst.h = 35;
	dst.x = RESOLUTION_WIDTH - 25 - 115 - 20 - dst.w;
	dst.y = RESOLUTION_HEIGHT - 25 - dst.h;

	actionButton_ = new ActionButton(ActionButtonType::DECK_SHUFFLE, renderer_, dst);
}

GameTable::~GameTable()
{
	TTF_CloseFont(textFont_);
}

void GameTable::handleEvents()
{
	player_->handleEvents();

	if (actionButton_->isSelected()) actionButtonPressed_ = true;
	else actionButtonPressed_ = false;
}

void GameTable::update(Mouse* mouse)
{
	player_->update(mouse);

	if (turnManager_.isPreparationTurn())
	{
		updatePreparationTurnReady();
	}

	checkNextActionAllowed();

	actionButton_->update(mouse);
}

void GameTable::checkNextActionAllowed() 
{
	if (turnManager_.isMyTurn())
	{
		if (playerCanDraw()) actionButton_->changeButtonType(ActionButtonType::DRAW);
		else if (playerCanPlayCard()) actionButton_->changeButtonType(ActionButtonType::PLAY_CARD);
		else actionButton_->changeButtonType(ActionButtonType::END_TURN);
	}
	else if (!turnManager_.isPreparationTurn())
	{
		actionButton_->changeButtonType(ActionButtonType::INACTIVE);
	}
}

void GameTable::clearTurnActions()
{
	playerDrawedThisTurn_ = false;
	playerPlayedCardThisTurn_ = false;
}

void GameTable::updatePreparationTurnReady()
{
	if (turnManager_.isClientPrepared() and turnManager_.isHostPrepared())
	{
		turnManager_.next();
	}
}

void GameTable::render()
{
	// Render background
	SDL_RenderCopy(renderer_, background_.get(), &sRect_, &dRect_);

	// PLAYER

	playerRenderDeck();

	playerRenderHand();


	// OPPONENT

	playerOpponentRenderDeck();

	playerOpponentRenderHand();


	renderPlayerNames();

	// Acction Button
	actionButton_->render();
}

void GameTable::renderPlayerNames()
{
	// Render player name
	SDL_Surface* surface = TTF_RenderText_Solid(textFont_, player_->getName().c_str(), { 0, 0, 0, 255 });
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_Rect playerDRect;
	playerDRect.x = 15;
	playerDRect.y = (RESOLUTION_HEIGHT / 2) + 10;
	playerDRect.w = surface->w;
	playerDRect.h = surface->h;

	SDL_RenderCopy(renderer_, textTexture, NULL, &playerDRect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textTexture);

	// Render opponent name
	surface = TTF_RenderText_Solid(textFont_, playerOpponent_->getName().c_str(), { 0, 0, 0, 255 });
	textTexture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_Rect opponentDRect;
	opponentDRect.x = 15;
	opponentDRect.y = (RESOLUTION_HEIGHT / 2) - surface->h - 10;
	opponentDRect.w = surface->w;
	opponentDRect.h = surface->h;

	SDL_RenderCopy(renderer_, textTexture, NULL, &opponentDRect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textTexture);

	// Render turn indicator 
	if (!turnManager_.isPreparationTurn())
	{
		if (turnManager_.isMyTurn())
		{
			SDL_Texture* turnIndicatorTexture = IMG_LoadTexture(renderer_, IMG_TURN_INDICATOR);
			playerDRect.y = playerDRect.y + playerDRect.h + 5;
			playerDRect.h = 5;

			SDL_RenderCopy(renderer_, turnIndicatorTexture, NULL, &playerDRect);

			SDL_DestroyTexture(turnIndicatorTexture);
		}
		//else {
		//	SDL_Texture* turnIndicatorTexture = IMG_LoadTexture(renderer_, IMG_TURN_INDICATOR);
		//	opponentDRect.y = playerDRect.y + playerDRect.h + 5;
		//	opponentDRect.h = 5;

		//	SDL_RenderCopy(renderer_, turnIndicatorTexture, NULL, &opponentDRect);

		//	SDL_DestroyTexture(turnIndicatorTexture);
		//}

		
	}

}

bool GameTable::isMyTurn() const
{
	return turnManager_.isMyTurn();
}

bool GameTable::actionButtonPressed() const
{
	return actionButtonPressed_;
}

ActionButtonType GameTable::actionButtonType() const
{
	return actionButton_->getType();
}

bool GameTable::preparationTurn() const
{
	return turnManager_.isPreparationTurn();
}

void GameTable::playerDraw(int cards)
{
	player_->drawFaceUp(cards);

	playerDrawedThisTurn_ = true;
}


std::vector<int> GameTable::playerDeckShuffle()
{
	actionButton_->changeButtonType(ActionButtonType::DRAW);

	return player_->shuffleDeck();
}

void GameTable::opponentPlayerDraw(int cardsDrawed)
{
	playerOpponent_->draw(cardsDrawed);
}

void GameTable::nextTurn()
{
	turnManager_.next();
}

void GameTable::clearButtonPressed()
{
	actionButtonPressed_ = false;
}

void GameTable::createOpponentDeck(std::vector<int>& cardIDs)
{
	playerOpponent_->loadDeck(cardIDs);
}

void GameTable::clientPlayerPreparationTurnReady()
{
	turnManager_.clientPreparationReady();
}

void GameTable::hostPlayerPreparationTurnReady()
{
	turnManager_.hostPreparationReady();
}

bool GameTable::playerCanDraw() const
{
	if (!playerDrawedThisTurn_ and player_->handSize() < 5 and player_->deckSize() > 0)
		return true;
	else
		return false;
}

bool GameTable::playerCanPlayCard() const
{
	if (playerPlayedCardThisTurn_) return false;
	else return true;
}

void GameTable::playerRenderDeck()
{
	SDL_Texture* deckTexture = IMG_LoadTexture(renderer_, IMG_DECK);
	SDL_Rect src, dst;
	src.x = 0;
	src.y = 0;
	src.w = 225;
	src.h = 310;

	dst.x = (int)(RESOLUTION_WIDTH - 225 * 0.5 - 25);
	dst.y = (int)(RESOLUTION_HEIGHT - 310 * 0.5 - 25);
	dst.w = (int)(225 * 0.5);
	dst.h = (int)(310 * 0.5);
	SDL_RenderCopy(renderer_, deckTexture, &src, &dst);

	// Player deck data(cards left)
	std::string text = std::to_string(player_->deckSize()) + "/" + std::to_string(player_->deckMaxSize());
	SDL_Surface* surface = TTF_RenderText_Solid(textFont_, text.c_str(), { 0, 0, 0, 255 });
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_, surface);
	dst.x = dst.x + (dst.w / 2) - (surface->w / 2);
	dst.y = dst.y + dst.h + 5;
	dst.w = surface->w;
	dst.h = surface->h;
	SDL_RenderCopy(renderer_, textTexture, NULL, &dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textTexture);
}

void GameTable::playerRenderHand()
{
	SDL_Rect src, dst;

	src.w = 215;
	src.h = 300;

	int startPointX = (RESOLUTION_WIDTH / 4); //horizontal
	int startPointY = (RESOLUTION_HEIGHT * 3 / 4); // vertical
	float textureProportion = 0.5f; // proporcion respecto al tamaño original de la imagen de textura
	int proportionalCardWidth = (int)(src.w * textureProportion); //107
	int cardMargin = 70;

	std::vector<Card>& hand = player_->hand();

	if (!hand.empty())
	{
		for (int i = 0; i < hand.size(); i++)
		{
			dst.x = startPointX + (i * proportionalCardWidth) + cardMargin; // 215 card default width +2 margin between cards
			dst.y = startPointY; // 3/4 of the screen heigth + 10 of margin
			// render a card
			hand.at(i).render(&dst, textureProportion);
		}
	}
}

void GameTable::playerOpponentRenderDeck()
{
	SDL_Texture* deckTexture = IMG_LoadTexture(renderer_, IMG_DECK);
	SDL_Rect src, dst;
	src.x = 0;
	src.y = 0;
	src.w = 225;
	src.h = 310;

	dst.x = 25;
	dst.y = 25;
	dst.w = (int)(225 * 0.5);
	dst.h = (int)(310 * 0.5);
	SDL_RenderCopy(renderer_, deckTexture, &src, &dst);

	// Opponent deck data(cards left)
	std::string text = std::to_string(playerOpponent_->deckSize()) + "/" + std::to_string(playerOpponent_->deckMaxSize());
	SDL_Surface*  surface = TTF_RenderText_Solid(textFont_, text.c_str(), { 0, 0, 0, 255 });
	SDL_Texture*  textTexture = SDL_CreateTextureFromSurface(renderer_, surface);
	dst.x = dst.x + (dst.w / 2) - (surface->w / 2);
	dst.y = dst.y + dst.h + 5;
	dst.w = surface->w;
	dst.h = surface->h;
	SDL_RenderCopy(renderer_, textTexture, NULL, &dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textTexture);
}

void GameTable::playerOpponentRenderHand()
{
	SDL_Rect src, dst;

	src.w = 215;
	src.h = 300;

	int startPointX = (RESOLUTION_WIDTH / 4); //horizontal
	int startPointY = 10; // vertical margin from top scren
	float textureProportion = 0.5f; // proporcion respecto al tamaño original de la imagen de textura
	int proportionalCardWidth = (int)(src.w * textureProportion); //107
	int cardMargin = 70;

	std::vector<Card>& hand = playerOpponent_->hand();

	for (int i = 0; i < hand.size(); i++)
	{
		dst.x = startPointX + (i * proportionalCardWidth) + cardMargin; // 215 card default width +2 margin between cards
		dst.y = startPointY; // 3/4 of the screen heigth + 10 of margin
		// render a card
		hand.at(i).render(&dst, textureProportion);
	}
}