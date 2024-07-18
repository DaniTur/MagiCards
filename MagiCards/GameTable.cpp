#include "GameTable.h"
#include <SDL_image.h>
#include "ResourcesList.h"


GameTable::GameTable(SDL_Renderer* renderer, Player* player, Player* opponent, Owner owner)
	: renderer_(renderer), player_(player), playerOpponent_(opponent)
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
	if (actionButton_->isSelected()) actionButtonPressed_ = true;
	else actionButtonPressed_ = false;
}

void GameTable::update(Mouse* mouse)
{
	player_->update(mouse);

	//selectedCardIndex_ = player_->selectedCard();

	actionButton_->update(mouse);
}

void GameTable::render()
{
	// Render background
	SDL_RenderCopy(renderer_, background_.get(), &sRect_, &dRect_);

	// PLAYER
	// Player deck
	// player_->renderDeck(SDL_Rect dstRect);
	//		
	// player_->renderHand(SDL_Rect dstRect); first card destination rectangle
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
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer_, surface);
	dst.x = dst.x + (dst.w / 2) - (surface->w / 2);
	dst.y = dst.y + dst.h + 5;
	dst.w = surface->w;
	dst.h = surface->h;
	SDL_RenderCopy(renderer_, textTexture, NULL, &dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textTexture);

	// Player hand cards
	src.w = 215;
	src.h = 300;

	std::vector<Card> hand = player_->hand();

	int startPointX = (RESOLUTION_WIDTH / 4); //horizontal
	int startPointY = (RESOLUTION_HEIGHT * 3 / 4); // vertical
	float textureProportion = 0.5f; // proporcion respecto al tamaño original de la imagen de textura
	int proportionalCardWidth = src.w * textureProportion; //107
	int cardMargin = 70;

	//dst.x = startPointX + (0 * proportionalCardWidth) + cardMargin;
	//dst.y = startPointY;

	//hand.at(0).render(&dst, textureProportion);

	//dst.x = startPointX + (1 * proportionalCardWidth) + cardMargin;
	//dst.y = startPointY;

	//hand.at(1).render(&dst, textureProportion);

	for (int i = 0; i < hand.size(); i++)
	{
		dst.x = startPointX + (i * proportionalCardWidth) + cardMargin; // 215 card default width +2 margin between cards
		dst.y = startPointY; // 3/4 of the screen heigth + 10 of margin
		// render a card
		hand[i].render(&dst, textureProportion);
	}

	// OPPONENT
	// Opponent deck
	//SDL_Rect dstOpponent;
	src.w = 225;
	src.h = 310;

	dst.x =  25;
	dst.y =  25;
	dst.w = (int)(225 * 0.5);
	dst.h = (int)(310 * 0.5);
	SDL_RenderCopy(renderer_, deckTexture, &src, &dst);

	// Opponent deck data(cards left)
	text = std::to_string(playerOpponent_->deckSize()) + "/" + std::to_string(playerOpponent_->deckMaxSize());
	surface = TTF_RenderText_Solid(textFont_, text.c_str(), { 0, 0, 0, 255 });
	textTexture = SDL_CreateTextureFromSurface(renderer_, surface);
	dst.x = dst.x + (dst.w / 2) - (surface->w / 2);
	dst.y = dst.y + dst.h + 5;
	dst.w = surface->w;
	dst.h = surface->h;
	SDL_RenderCopy(renderer_, textTexture, NULL, &dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(textTexture);

	// Opponent hand cards
	hand = playerOpponent_->hand();

	startPointY = 10; // vertical margin from top scren

	for (int i = 0; i < hand.size(); i++)
	{
		dst.x = startPointX + (i * proportionalCardWidth) + cardMargin; // 215 card default width +2 margin between cards
		dst.y = startPointY; // 3/4 of the screen heigth + 10 of margin
		// render a card
		hand[i].render(&dst, textureProportion);
	}

	// Acction Button
	actionButton_->render();
}

bool GameTable::isMyTurn() const
{
	return turn_;
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
	return turn_ == -1;
}

void GameTable::playerDraw(int cards)
{
	player_->drawFaceUp(cards);
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
	turn_++;
}

void GameTable::clearButtonPressed()
{
	actionButtonPressed_ = false;
}

void GameTable::createOpponentDeck(std::vector<int>& cardIDs)
{
	playerOpponent_->loadDeck(cardIDs);
}
