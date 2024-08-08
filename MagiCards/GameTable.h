#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "Player.h"
#include "ActionButton.h"
#include "Mouse.h"
#include "TurnManager.h"
#include "Common.h"

constexpr auto RESOLUTION_WIDTH = 1280;
constexpr auto RESOLUTION_HEIGHT = 720;


class GameTable
{
public:

	GameTable(SDL_Renderer* renderer, Player* player, Player* opponent, OWNER owner);

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

	void clientPlayerPreparationTurnReady();

	void hostPlayerPreparationTurnReady();

	bool playerCanDraw() const;

	bool playerCanPlayCard() const;

	void clearTurnActions();

private:

	void playerRenderDeck();

	void playerRenderHand();

	void playerOpponentRenderDeck();

	void playerOpponentRenderHand();

	void updatePreparationTurnReady();

	void renderPlayerNames();

	void checkNextActionAllowed();

private:
	
	SDL_Renderer* renderer_;

	using TextureDestructor = std::function<void(SDL_Texture*)>;
	std::unique_ptr<SDL_Texture, TextureDestructor> background_;

	SDL_Rect sRect_, dRect_; //background rectangle

	TTF_Font* textFont_ = nullptr;

	Player  *player_, *playerOpponent_;

	ActionButton* actionButton_;
	bool actionButtonPressed_ = false;

	int selectedCardIndex_ = -1; // seleccted card index of the hand vector

	//_inGameMenu

	OWNER owner_;

	TurnManager turnManager_;

	bool playerDrawedThisTurn_ = false;
	bool playerPlayedCardThisTurn_ = false;
};

