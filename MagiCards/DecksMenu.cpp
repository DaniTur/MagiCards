#include "DecksMenu.h"
#include <SDL_image.h>
#include "ResourcesList.h"

DecksMenu::DecksMenu()
{
}

DecksMenu::DecksMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

	_background = IMG_LoadTexture(_renderer, IMG_DECK_MENU);

	_sRect = {0, 0, 1000, 1000}; // x, y, width, height

	_dRect = {0, 0, windowW , windowH};

	SDL_Rect dst;
	dst.x = (windowW / 2) - (250);
	dst.y = 500;
	dst.w = 500;
	dst.h = 100;
	_backButton = new Button("", _renderer, {0, 400}, dst);
}

DecksMenu::~DecksMenu()
{
}

void DecksMenu::handleEvents()
{
    if (_backButton->isSelected()) _buttonSelected = 0;
    else _buttonSelected = -1;
}

void DecksMenu::update(Mouse* mouse)
{
    _backButton->update(mouse);
}

void DecksMenu::render()
{
    //render background
    SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

    _backButton->render();
}

int DecksMenu::menuType()
{
    return _MENU_TYPE;
}

int DecksMenu::getButtonPressed()
{
    return _buttonSelected;
}

void DecksMenu::clearPressedButton()
{
    _buttonSelected = -1;
}

void DecksMenu::handleTextInputEvent(SDL_TextInputEvent event)
{
}
