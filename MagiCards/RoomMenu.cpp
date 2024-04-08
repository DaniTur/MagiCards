#include "RoomMenu.h"
#include <SDL_image.h>

RoomMenu::RoomMenu(SDL_Renderer* renderer, Player* playerHost, bool serverSide) 
	: _renderer(renderer), _playerHost(playerHost), _serverSide(serverSide)
{
	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, "RoomMenu.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;

	_startButton = new Button("StartGame", _renderer, 0, 600);
	_startButton->setWindowXY((windowW / 2) - (250), 450);

	_backButton = new Button("Back", _renderer, 0, 400);
	_backButton->setWindowXY((windowW / 2) - (250), 580);
}

RoomMenu::~RoomMenu()
{
	delete _startButton;
	delete _backButton;
	SDL_DestroyTexture(_background);
}

void RoomMenu::handleEvents()
{
	if (_startButton->isSelected()) _buttonSelected = 0;
	else if (_backButton->isSelected()) _buttonSelected = 1;
	else _buttonSelected = -1;
}

void RoomMenu::handleTextInputEvent(SDL_TextInputEvent event)
{
}

void RoomMenu::update(Mouse* mouse)
{
	if (_serverSide)
	{
		_startButton->update(mouse);
	}
	_backButton->update(mouse);
}

void RoomMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	if (_serverSide)
	{
		_startButton->render();
	}
	_backButton->render();
}

int RoomMenu::menuType()
{
    return _MENU_TYPE;
}

int RoomMenu::getButtonPressed()
{
    return _buttonSelected;
}

void RoomMenu::clearPressedButton()
{
	_buttonSelected = -1;
}

bool RoomMenu::serverSide()
{
	return _serverSide;
}
