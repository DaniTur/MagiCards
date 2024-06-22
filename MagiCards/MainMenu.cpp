#include "MainMenu.h"
#include <SDL_image.h>
#include <iostream>
#include "ResourcesList.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

MainMenu::MainMenu()
{
}

MainMenu::MainMenu(SDL_Renderer* renderer) :  _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, IMG_MAIN_MENU);
	
	_sRect = {0, 0, 1000, 1000}; // x, y, width, height

	_dRect = {0, 0, windowW, windowH};

	SDL_Rect dst;
	dst.x = (windowW / 2) - (250); // 250 is half the button texture width
	dst.y = 0 + 50;
	dst.w = 500;
	dst.h = 100;
	_buttons[CREATE_ROOM_BTN] = new Button("", _renderer, { 0, 0 }, dst);

	dst.y = 110 + 50;
	_buttons[JOIN_ROOM_BTN] = new Button("", _renderer, {0, 100}, dst);

	dst.y = 220 + 50;
	_buttons[DECKS_BTN] = new Button("", _renderer, {0, 200}, dst);

	dst.y = 330 + 50;
	_buttons[QUIT_GAME_BTN] = new Button("", _renderer, {0, 300}, dst);
}

MainMenu::~MainMenu()
{
	SDL_DestroyTexture(_background);
	for (Button* btn : _buttons)
	{
		delete btn;
	}
}

void MainMenu::handleEvents()
{
	bool anyButtonSelected = false;
	int i = 0;
	//@pre: only one button could be selected at time
	while (i < MAX_BUTTONS && !anyButtonSelected) {
		if (_buttons[i]->isSelected()) anyButtonSelected = true;
		else i++;
	}

	_buttonSelected = anyButtonSelected ? i : -1;
}

void MainMenu::update(Mouse* mouse)
{
	//update all menu components
	for (Button* btn : _buttons)
	{
		btn->update(mouse);
	}
}

void MainMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	//render all menu components
	for (Button* btn : _buttons)
	{
		btn->render();
	}
}

int MainMenu::getButtonPressed()
{
	return _buttonSelected;
}

int MainMenu::menuType()
{
	return _MENU_TYPE;
}

void MainMenu::clearPressedButton()
{
	_buttonSelected = -1;
}

void MainMenu::handleTextInputEvent(SDL_TextInputEvent event)
{
}
