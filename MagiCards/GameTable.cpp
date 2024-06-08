#include "GameTable.h"
#include <SDL_image.h>
#include "ResourcesList.h"

GameTable::GameTable(SDL_Renderer* renderer, Player* player, Player* opponent, Owner owner)
	: renderer_(renderer), player_(player), playerOpponent_(opponent)
{
	const int windowW = 1280;
	const int windowH = 720;

	// testing, remove
	//if (owner == Owner::server)
	//{
	//	background_ = IMG_LoadTexture(renderer_, IMG_GAME_TABLE);
	//}
	//else
	//{
	//	background_ = IMG_LoadTexture(renderer_, "D:\\MagiCardsProject\\MagiCards\\resources\\gameTable2.png");
	//}
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
}

GameTable::~GameTable()
{
	//SDL_DestroyTexture(background_);
}

void GameTable::handleEvents()
{
}

void GameTable::update()
{
	if (turn_ == 0) // starting turn
	{
		// shuffle deck
		player_->shuffleDeck();

		// draw first 5 cards to hand, and remove them from the deck
		player_->draw(5);
		turn_++;
	}
	//else if (true) // if my turn
	//{

	//}
}

void GameTable::render()
{
	// Render background
	SDL_RenderCopy(renderer_, background_, &sRect_, &dRect_);

	// PLAYER
	// Player deck
	SDL_Texture* background = IMG_LoadTexture(renderer_, IMG_DECK);
	SDL_Rect src, dst;
	src.x = 0;
	src.y = 0;
	src.w = 225;
	src.h = 310;

	dst.x = 1280 - 225 * (0.5) - 25;
	dst.y = 720 - 310 * (0.5) - 25;
	dst.w = 225 * (0.5);
	dst.h = 310 * (0.5);
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
		hand[i].render(&dst, 0.15);
	}

	// OPPONENT
	// Opponent deck
	//SDL_Rect dstOpponent;
	dst.x =  25;
	dst.y =  25;
	dst.w = 225 * (0.50);
	dst.h = 310 * (0.50);
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
}

bool GameTable::isMyTurn() const
{
	return turn_;
}

SDL_Texture* GameTable::getBackground() const
{
	return background_;
}
