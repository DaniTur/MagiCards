#include <SDL_image.h>
#include "JoinRoomMenu.h"


JoinRoomMenu::JoinRoomMenu()
{
}

JoinRoomMenu::JoinRoomMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

    _background = IMG_LoadTexture(_renderer, "JoinRoomMenu.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;

	_backButton = new Button("Back", _renderer, 0, 400);
	_backButton->setWindowXY((windowW / 2) - (250), 500);
}

JoinRoomMenu::~JoinRoomMenu()
{
	delete _backButton;
	SDL_DestroyTexture(_background);
}

void JoinRoomMenu::handleEvents()
{
	if (_backButton->isSelected()) _buttonSelected = 0;
	else _buttonSelected = -1;
}

void JoinRoomMenu::update(Mouse* mouse)
{
	_backButton->update(mouse);
}

void JoinRoomMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	_backButton->render();
}

int JoinRoomMenu::menuType()
{
	return _MENU_TYPE;
}

int JoinRoomMenu::getButtonPressed()
{
	return _buttonSelected;
}

void JoinRoomMenu::clearPressedButton()
{
	_buttonSelected = -1;
}

void JoinRoomMenu::handleTextInputEvent(SDL_TextInputEvent event)
{
}
