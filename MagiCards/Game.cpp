#include "Game.h"
#include "SDLException.h"
#include "Connection.h"
#include "MainMenu.h" //also includes Menu
#include "CreateRoomMenu.h" //TODO: refactor menu creations to MenuFactoryMethod
#include "JoinRoomMenu.h"
#include "DecksMenu.h"

Game::Game() {
	_window = NULL;
	_renderer = NULL;
	_isRunning = true;
}

Game::~Game()
{
}

void Game::init()
{
	initSDL();
	createWindowAndRenderer();
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	_isRunning = true;

	//Main Menu
	Menu* mainMenu = new MainMenu(_renderer);
	_menuStack.push(mainMenu);
	_activeMenu = true;

	static Mouse* m = new Mouse(_renderer);
	_mouse = m;
}

void Game::initSDL()
{
	bool isInitialized = SDL_Init(SDL_INIT_VIDEO) == 0;
	if (!isInitialized) throw SDLException(SDL_GetError());
}

bool Game::isRunning() {
	return _isRunning;
}

void Game::createWindowAndRenderer()
{
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &_window, &_renderer);
	SDL_SetWindowTitle(_window, "MagiCards");
	if (_window == NULL || _renderer == NULL) throw SDLException(SDL_GetError());
}

void Game::handleEvents()
{
	SDL_GetMouseState(&(_mouse->getCursor()->x), &(_mouse->getCursor()->y));
	_mouse->setTipXY(_mouse->getCursor()->x, _mouse->getCursor()->y);

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			_isRunning = false;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (_activeMenu)
				{
					(_menuStack.top())->handleEvents();
				}
			}
			
		default:
			break;
		}

	}
}

void Game::update()
{
	if (_activeMenu)
	{
		updateMenu();
	}
	else
	{
		//update game
	}
}

void Game::updateMenu()
{
	Menu* tmpMenu = NULL;
	Menu* activeMenu = _menuStack.top();
	activeMenu->update(_mouse);

	switch (activeMenu->menuType())
	{
	case 0://Main menu
		switch (activeMenu->getButtonPressed())
		{
		case CREATE_ROOM:
			tmpMenu = new CreateRoomMenu(_renderer);
			_menuStack.push(tmpMenu);
			std::cout << "Game update: Main Menu -> Create Room" << std::endl;
			break;
		case JOIN_ROOM:
			tmpMenu = new JoinRoomMenu(_renderer);
			_menuStack.push(tmpMenu);
			std::cout << "Game update: Main Menu -> Join Room" << std::endl;
			break;
		case DECKS:
			tmpMenu = new DecksMenu(_renderer);
			_menuStack.push(tmpMenu);
			std::cout << "Game update: Main Menu -> Decks" << std::endl;
			break;
		case QUIT_GAME:
			_isRunning = false;
			std::cout << "Game update: Main Menu -> Quit game" << std::endl;
			break;
		}
		break;
	case 1: //Create Room menu
		switch (activeMenu->getButtonPressed())
		{
		case 0: // Back button
			_menuStack.pop();
			break;
		}
		break;
	case 2: //Join Room menu
		switch (activeMenu->getButtonPressed())
		{
		case 0: // Back button
			_menuStack.pop();
			break;
		}
		break;
	case 3: //Decks Menu
		switch (activeMenu->getButtonPressed())
		{
		case 0: // Back button
			_menuStack.pop();
			break;
		}
		break;
	}

	activeMenu->clearPressedButton();
}

void Game::render()
{	
	// if: there is a menu active, render it
	// else: render the game
	if (_activeMenu)
	{
		(_menuStack.top())->render();
	}
	else
	{
		// render game
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	}

	_mouse->render();

	SDL_RenderPresent(_renderer);
	//SDL_SetRenderDrawColor(_renderer, 20, 20, 20, 255);
	SDL_RenderClear(_renderer);
	
	//SDL_RenderClear(_renderer);
	//renderizar imagenes
	//SDL_RenderPresent(_renderer); //muestra o "pinta" el render

}

void Game::release()
{
	_connection.clear();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Game::createGameRoom() {
	std::cout << "creating a server socket" << std::endl;
	Connection connection;
	if (connection.startServerConnection() != 0)
		std::cout << "Throw ConnectionException at server" << std::endl;
	
}

void Game::joinGameRoom() {
	std::string ip;
	int port;

	std::cout << "Introduce the IP adress: " << std::endl;
	std::cin >> ip;

	std::cout << "Introduce the Port: " << std::endl;
	std::cin >> port;
	//TODO: check the input format

	std::cout << "creating a client socket" << std::endl;
	Connection connection;
	if (connection.startClientConnection(ip.c_str(), port) != 0)
		std::cout << "Throw ConnectionException at client" << std::endl;
}