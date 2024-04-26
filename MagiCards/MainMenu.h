#pragma once
#include "Button.h"
#include "Menu.h"
#include "Mouse.h"

const int MAX_BUTTONS = 4;
enum ButtonNames { CREATE_ROOM_BTN, JOIN_ROOM_BTN, DECKS_BTN, QUIT_GAME_BTN};

class MainMenu : public Menu
{
public:
	MainMenu();

	MainMenu(SDL_Renderer* renderer);
	
	~MainMenu();

	void handleEvents() override;

	void update(Mouse* mouse) override;

	void render() override;

	int getButtonPressed() override; //return index of the pressed button of the list

	int menuType() override;

	void clearPressedButton() override;

private:
	const int _MENU_TYPE = 0;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;
	
	Button* _buttons[MAX_BUTTONS];
	int _buttonSelected = -1;
	// Inherited via Menu
	void handleTextInputEvent(SDL_TextInputEvent event) override;
	// default: -1 no buttonSelected
};

