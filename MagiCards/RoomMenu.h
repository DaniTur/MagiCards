#pragma once
#include "Menu.h"
#include "Button.h"

class RoomMenu :  public Menu
{
public:
	RoomMenu();

	RoomMenu(SDL_Renderer* renderer);

	~RoomMenu();

	// Inherited via Menu
	void handleEvents() override;

	void handleTextInputEvent(SDL_TextInputEvent event) override;

	void update(Mouse* mouse) override;

	void render() override;

	int menuType() override;

	int getButtonPressed() override;

	void clearPressedButton() override;

private:
	const int _MENU_TYPE = 4;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	Button *_backButton, *_startButton;
	int _buttonSelected = -1; // default: -1 no buttonSelected
};

