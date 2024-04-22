#pragma once
#include "Menu.h"
#include "Mouse.h"
#include "Button.h"
#include "TextInput.h"
#include <vector>

class JoinRoomMenu : public Menu
{
public:
	JoinRoomMenu();

	JoinRoomMenu(SDL_Renderer* renderer);

	~JoinRoomMenu();

	void handleEvents() override;

	void update(Mouse* mouse) override;

	void render() override;

	int menuType() override;

	int getButtonPressed() override;

	void clearPressedButton() override;

	void handleTextInputEvent(SDL_TextInputEvent event) override;

	void handleKeyDownEvent(SDL_Keysym keysym);

	std::string getPlayerName();

	int getSelectedDeck();

	std::string getServerAddress();

	int getServerPort();

private:
	const int _MENU_TYPE = 2;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	Button* _backButton, *_joinButton;
	int _buttonSelected = -1; // default: -1 no buttonSelected

	std::vector<TextInput*> _textInputs;
	enum {PLAYER_NAME, DECK_SELECTOR, SERVER_IP, SERVER_PORT};
	bool isValidText(const char text);
};

