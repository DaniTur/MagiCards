#include "RoomMenu.h"
#include <SDL_image.h>


RoomMenu::RoomMenu(SDL_Renderer* renderer, Player* player, bool serverSide) 
	: _renderer(renderer), _serverSide(serverSide)
{
	if (serverSide) _playerHost = player;
	else _playerClient = player;

	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, "D:\\MagiCardsProject\\MagiCards\\MagiCards\\RoomMenu.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;

	_dTextRect.w = 0;
	_dTextRect.h = 0;
	_dTextRect.x = 0;
	_dTextRect.y = 0;


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
	//TODO: check if there are 2 players joined to the room
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

	//render connection info to share

	//render player (name + deck)


	if (_playerHostConnected)
	{
		_surfaceText = TTF_RenderText_Solid(_textFont, _playerHost->getName().c_str(), _textColor);
		_dTextRect.w = _surfaceText->w;
		_dTextRect.h = _surfaceText->h;
		_dTextRect.x = 50;
		_dTextRect.y = 50;
		_textTexture = SDL_CreateTextureFromSurface(_renderer, _surfaceText);
		SDL_FreeSurface(_surfaceText);
		SDL_RenderCopy(_renderer, _textTexture, NULL, &_dTextRect);
	}

	if (_playerClientConnected)
	{
		_surfaceText = TTF_RenderText_Solid(_textFont, _playerClient->getName().c_str(), _textColor);
		_dTextRect.w = _surfaceText->w;
		_dTextRect.h = _surfaceText->h;
		_dTextRect.x = 50;
		_dTextRect.y = 80;
		_textTexture = SDL_CreateTextureFromSurface(_renderer, _surfaceText);
		SDL_FreeSurface(_surfaceText);
		SDL_RenderCopy(_renderer, _textTexture, NULL, &_dTextRect);
	}


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


void RoomMenu::playerHostConnected()
{
	_playerHostConnected = true;
}

void RoomMenu::playerClientConnected() {
	_playerClientConnected = true;
}

void RoomMenu::playerHostDisconnected()
{
	_playerHostConnected = false;
}
