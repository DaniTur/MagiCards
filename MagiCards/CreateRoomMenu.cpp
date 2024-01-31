#include "CreateRoomMenu.h"
#include <SDL_image.h>
#include <iostream>

CreateRoomMenu::CreateRoomMenu()
{
}

CreateRoomMenu::CreateRoomMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	_background = IMG_LoadTexture(_renderer, "CreateRoomMenu.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = 800;
	_dRect.h = 600;

	const int windowX = 800;
	const int windowY = 600;

	_backButton = new Button("Back", _renderer, 0, 400);
	_backButton->setWindowXY((windowX / 2) - (250), 500);

	_playerNumer = 1;
}

CreateRoomMenu::~CreateRoomMenu()
{
	delete _backButton;
	SDL_DestroyTexture(_background);
}

void CreateRoomMenu::handleEvents()
{
	if (_backButton->isSelected()) _buttonSelected = 0;
	else _buttonSelected = -1;
	std::cout << "CreateRoomMenu button selected: " << _buttonSelected << std::endl;
}

void CreateRoomMenu::update(Mouse* mouse)
{
	_backButton->update(mouse);
}

void CreateRoomMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	// render Text: your ip is: 127.0.0.1
	// render Text: your port number is: 30000
	// render Text: players 1/2
	//if (_playerNumer < _MAX_PLAYERS)
	//{
	//	 render Text: Waiting players...
	//}
	_backButton->render();
}

int CreateRoomMenu::menuType()
{
	return _MENU_TYPE;
}

int CreateRoomMenu::getButtonPressed()
{
	return _buttonSelected;
}

void CreateRoomMenu::clearPressedButton()
{
	_buttonSelected = -1;
}
