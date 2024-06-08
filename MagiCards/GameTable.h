#pragma once
#include <SDL.h>
#include "Player.h"
#include <SDL_ttf.h>

class GameTable
{
public:

	// Testing, remove
	enum class Owner {
		server,
		client
	};

	GameTable(SDL_Renderer* renderer, Player* player, Player* opponent, Owner owner);

	~GameTable();

	void handleEvents();

	void update();
	
	void render();

	//return true if the turn belongs to the player side calling the function
	bool isMyTurn() const; 

	SDL_Texture* getBackground() const; // testing, remove

private:
	
	SDL_Renderer* renderer_;

	SDL_Texture* background_ = nullptr;
	SDL_Rect sRect_, dRect_; //background rectangle

	TTF_Font* textFont_ = nullptr;

	//	_dividerLine

	Player  *player_, *playerOpponent_;
	//	_playerTableCards
	//	_playerManaResource

	//	Button* _actionButton;

	//_oponentTableCards
	//_oponentManaResource

	//_inGameMenu

	static inline uint8_t turn_ = 0; // starts with value zero
};

