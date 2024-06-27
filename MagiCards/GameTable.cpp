#include "GameTable.h"
#include <SDL_image.h>
#include "ResourcesList.h"


GameTable::GameTable(SDL_Renderer* renderer, Player* player, Player* opponent, Owner owner)
	: renderer_(renderer), player_(player), playerOpponent_(opponent)
{
	const int windowW = 1280;
	const int windowH = 720;

	background_ = IMG_LoadTexture(renderer_, IMG_GAME_TABLE);

	sRect_.x = 0;
	sRect_.y = 0;
	sRect_.w = 1280;
	sRect_.h = 720;

	dRect_.x = 0;
	dRect_.y = 0;
	dRect_.w = windowW;
	dRect_.h = windowH;

	textFont_ = TTF_OpenFont(TEXT_FONT, 14);

	SDL_Rect dst;
	// 25 right marging deck to screen, 115 deck aprox, 20 maring to deck
	dst.w = 70;
	dst.h = 35;
	dst.x = windowW - 25 - 115 - 20 - dst.w; 
	dst.y = windowH - 25 - dst.h;

	actionButton_ = new ActionButton(ActionButtonType::DECK_SHUFFLE, renderer_, dst);
}

GameTable::~GameTable()
{
	SDL_DestroyTexture(background_);
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
	SDL_RenderCopy(renderer_, background_, &sRect_, &dRect_);

	// PLAYER
	// Player deck
	// player_->renderDeck(SDL_Rect dstRect);
	//		
	// player_->renderHand(SDL_Rect dstRect); first card destination rectangle
	SDL_Texture* background = IMG_LoadTexture(renderer_, IMG_DECK);
	SDL_Rect src, dst;
	src.x = 0;
	src.y = 0;
	src.w = 225;
	src.h = 310;

	dst.x = (int)(1280 - 225 * 0.5 - 25);
	dst.y = (int)(720 - 310 * 0.5 - 25);
	dst.w = (int)(225 * 0.5);
	dst.h = (int)(310 * 0.5);
	SDL_RenderCopy(renderer_, background, &src, &dst);

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
	std::vector<Card> hand = player_->hand();
	for (int i = 0; i < hand.size(); i++)
	{
		SDL_Rect dst;
		dst.x = (1280 / 4) + (i * 215) + 2; // 215 card default width +2 margin between cards
		dst.y = (720 * 3 / 4) + 10; // 3/4 of the screen heigth + 10 of margin
		// render a card
		hand[i].render(&dst, 0.15f);
	}

	// OPPONENT
	// Opponent deck
	//SDL_Rect dstOpponent;
	dst.x =  25;
	dst.y =  25;
	dst.w = (int)(225 * 0.5);
	dst.h = (int)(310 * 0.5);
	SDL_RenderCopy(renderer_, background, &src, &dst);

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

	// Acction Button
	actionButton_->render();
}

bool GameTable::isMyTurn() const
{
	return turn_;
}

SDL_Texture* GameTable::getBackground() const
{
	return background_;
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
	player_->draw(cards);
}

void GameTable::playerDeckShuffle()
{
	player_->shuffleDeck();
	actionButton_->changeButtonType(ActionButtonType::DRAW);
	std::cout << "action button type changed to DRAW" << std::endl;
}

void GameTable::opponentPlayerDraw(int cardsDrawed)
{
	// nosotros no sabemos que le ha tocado al mezclar al rival, si nos dice que ha robado 5, no sabemos cuales cartas, por lo tanto tenemos que restar al deck size del oponente
	// el numero de cartas que nos envia por red que ha robado, 
	playerOpponent_.updateDeckSize(cardsDrawed);
}

void GameTable::nextTurn()
{
	turn_++;
}

void GameTable::clearButtonPressed()
{
	actionButtonPressed_ = false;
}
