#pragma once
#include <SDL.h>

class GameTable
{
public:
	GameTable(SDL_Renderer* renderer);

	~GameTable();

	void handleEvents();

	void update();
	
	void render();

private:
	SDL_Renderer* _renderer;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;

	//	_dividerLine

	//	_player
	//	_playerDeckCards
	//	_playerHandCards
	//	_playerTableCards
	//	_playerManaResource

	//	Button* _actionButton;

	//_oponentDeckCards
	//_oponentHandCards
	//_oponentTableCards
	//_oponentManaResource

	//_inGameMenu

};

