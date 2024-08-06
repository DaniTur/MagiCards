#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "Player.h"
#include "ActionButton.h"
#include "Mouse.h"

constexpr auto RESOLUTION_WIDTH = 1280;
constexpr auto RESOLUTION_HEIGHT = 720;

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

	void update(Mouse* mouse);
	
	void render();

	//return true if the turn belongs to the player side calling the function
	bool isMyTurn() const; 

	bool actionButtonPressed() const;

	ActionButtonType actionButtonType() const;

	bool preparationTurn() const;

	void playerDraw(int cards);

	std::vector<int> playerDeckShuffle();

	void opponentPlayerDraw(int cardsDrawed);

	void nextTurn();

	void clearButtonPressed();

	void createOpponentDeck(std::vector<int>& cardIDs);

private:

	void playerRenderDeck();

	void playerRenderHand();

	void playerOpponentRenderDeck();

	void playerOpponentRenderHand();

private:
	
	SDL_Renderer* renderer_;

	using TextureDestructor = std::function<void(SDL_Texture*)>;
	std::unique_ptr<SDL_Texture, TextureDestructor> background_;

	SDL_Rect sRect_, dRect_; //background rectangle

	TTF_Font* textFont_ = nullptr;

	//Player  *player_, *playerOpponent_;
	std::unique_ptr<Player> player_, playerOpponent_;
	//	_playerTableCards
	//	_playerManaResource

	ActionButton* actionButton_;
	bool actionButtonPressed_ = false;

	int selectedCardIndex_ = -1; // seleccted card index of the hand vector

	//_oponentTableCards
	//_oponentManaResource

	//_inGameMenu

	int turn_ = -1; // preparation turn
	bool playerReady = false;
	bool opponentPlayerReady = false;
};

