#include "CreateRoomMenu.h"
#include <SDL_image.h>
#include <iostream>

CreateRoomMenu::CreateRoomMenu()
{
}

CreateRoomMenu::CreateRoomMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, "CreateRoomMenu.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;

	_createButton = new Button("Create", _renderer, 0, 500);
	_createButton->setWindowXY((windowW / 2) - (250), 450);

	_backButton = new Button("Back", _renderer, 0, 400);
	_backButton->setWindowXY((windowW / 2) - (250), 580);

	_playerNameInput = new TextInput(_renderer, (windowW / 2) - (175), 100, "player1");
	_deckSelector = new TextInput(_renderer, (windowW / 2) - (175), 300, "deck index");
}

CreateRoomMenu::~CreateRoomMenu()
{
	delete _createButton;
	delete _backButton;
	SDL_DestroyTexture(_background);
}

void CreateRoomMenu::handleEvents()
{
	if (_playerNameInput->isSelected()) {
		_deckSelector->unfocus();
		_playerNameInput->focus();
	}
	else if (_deckSelector->isSelected())
	{
		_playerNameInput->unfocus();
		_deckSelector->focus();
	}
	else {
		_playerNameInput->unfocus();
		_deckSelector->unfocus();

		if (_createButton->isSelected()) _buttonSelected = 0;
		else if (_backButton->isSelected()) _buttonSelected = 1;
		else _buttonSelected = -1;
	}
}

void CreateRoomMenu::handleTextInputEvent(SDL_TextInputEvent textEvent)
{
	if (_playerNameInput->isFocused())
	{
		_playerNameInput->addText(textEvent.text);
	}
	else if (_deckSelector->isFocused())
	{
		_deckSelector->addText(textEvent.text);
	}
}

void CreateRoomMenu::update(Mouse* mouse)
{
	if (_playerNameInput->isFocused())
	{
		_playerNameInput->startTextInput();
	}
	else if (_deckSelector->isFocused())
	{
		_deckSelector->startTextInput();
	}
	else
	{
		_playerNameInput->stopTextInput();
		_deckSelector->stopTextInput();
	}

	_playerNameInput->update(mouse);
	_deckSelector->update(mouse);
	_createButton->update(mouse);
	_backButton->update(mouse);
}

void CreateRoomMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	_deckSelector->render();
	_playerNameInput->render();
	_createButton->render();
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



