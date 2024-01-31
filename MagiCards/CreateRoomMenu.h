#pragma once
#include "Menu.h"
#include "Mouse.h"
#include "Button.h"

class CreateRoomMenu : public Menu
{
public:
	CreateRoomMenu();

	CreateRoomMenu(SDL_Renderer* renderer);

	~CreateRoomMenu();

	void handleEvents() override;

	void update(Mouse* mouse) override;

	void render() override;

	int menuType() override;

	int getButtonPressed() override;

	void clearPressedButton() override;

private:
	const int _MENU_TYPE = 1;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	Button* _backButton;
	int _buttonSelected = -1; // default: -1 no buttonSelected

	const int _MAX_PLAYERS = 2;
	int _playerNumer;
};

