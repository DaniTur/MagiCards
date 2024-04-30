#pragma once
#include "Menu.h"
#include "Button.h"
#include "Player.h"
#include "Connection.h"
#include <SDL_ttf.h>

class RoomMenu :  public Menu
{
public:
	RoomMenu();

	RoomMenu(SDL_Renderer* renderer, Player* playerHost, bool serverSide);

	~RoomMenu();

	// Inherited via Menu
	void handleEvents() override;

	void handleTextInputEvent(SDL_TextInputEvent event) override;

	void update(Mouse* mouse) override;

	void render() override;

	int menuType() override;

	int getButtonPressed() override;

	void clearPressedButton() override;

	bool serverSide();

	void playerHostConnected();

	void playerClientConnected();

	void playerHostDisconnected();

	void playerClientDisconnected();

private:
	const int _MENU_TYPE = 4;

	SDL_Texture* _background;
	SDL_Rect _sRect, _dRect;
	SDL_Renderer* _renderer;

	Button *_backButton, *_startButton;
	int _buttonSelected = -1; // default: -1 no buttonSelected

	TTF_Font* _textFont = TTF_OpenFont("D:\\MagiCardsProject\\MagiCards\\MagiCards\\OpenSans-Regular.ttf", 18);
	SDL_Color _textColor = { 0, 0, 0 }; // black
	SDL_Surface* _surfaceText;
	SDL_Texture* _textTexture;
	SDL_Rect _dTextRect;

	Player *_playerHost, *_playerClient;
	bool _serverSide = false;
	Connection* _connection;

	bool _playerHostConnected = false;
	bool _playerClientConnected = false;

	//client info about server
	std::string endpointAdress;
	int endpointPort;
};

