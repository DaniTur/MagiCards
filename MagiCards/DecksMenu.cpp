#include "DecksMenu.h"
#include <SDL_image.h>
#include "ResourcesList.h"

DecksMenu::DecksMenu()
{
}

DecksMenu::DecksMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	_background = IMG_LoadTexture(_renderer, IMG_DECK_MENU);

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
