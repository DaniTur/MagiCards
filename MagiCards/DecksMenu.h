#pragma once
#include "Menu.h"
#include "Button.h"

class DecksMenu : public Menu
{
public:
	DecksMenu();
	
	DecksMenu(SDL_Renderer* renderer);

	~DecksMenu();

	// Inherited via Menu
	void handleEvents() override;
	void update(Mouse* mouse) override;
	void render() override;
	int menuType() override;
	int getButtonPressed() override;
	void clearPressedButton() override;

private:
	const int _MENU_TYPE = 3;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	Button* _backButton;
	int _buttonSelected = -1;
	// Inherited via Menu
	void handleTextInputEvent(SDL_TextInputEvent event) override;
	// default: -1 no buttonSelected
};

