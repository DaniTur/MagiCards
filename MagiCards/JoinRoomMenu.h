#pragma once
#include "Menu.h"
#include "Button.h"

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

private:
	const int _MENU_TYPE = 2;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	Button* _backButton;
	int _buttonSelected = -1; // default: -1 no buttonSelected
};

