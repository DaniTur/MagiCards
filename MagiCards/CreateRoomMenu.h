#pragma once
#include "Menu.h"
#include "Mouse.h"
#include "Button.h"
#include "TextInput.h"

class CreateRoomMenu : public Menu
{
public:
	CreateRoomMenu();

	CreateRoomMenu(SDL_Renderer* renderer);

	~CreateRoomMenu();

	void handleEvents() override;

	void handleTextInputEvent(SDL_TextInputEvent event) override;

	void handleKeyDownEvent(SDL_Keysym keysym);

	void update(Mouse* mouse) override;

	void render() override;

	int menuType() override;

	int getButtonPressed() override;

	void clearPressedButton() override;

	std::string getPlayerName();

	int getSelectedDeck();

private:
	const int _MENU_TYPE = 1;

	SDL_Texture* _background = NULL;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer = NULL;

	Button *_backButton = NULL, *_createButton = NULL;
	int _buttonSelected = -1; // default: -1 no buttonSelected

	TextInput* _playerNameInput = NULL;
	TextInput* _deckSelector = NULL; //should be replaced by a proper selector

	bool isValidText(const char text);
};

