#include <SDL_image.h>
#include "JoinRoomMenu.h"


JoinRoomMenu::JoinRoomMenu()
{
}

JoinRoomMenu::JoinRoomMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

    _background = IMG_LoadTexture(_renderer, "JoinRoomMenu.png");

	_sRect.x = 0;
	_sRect.y = 0;
	_sRect.w = 1000;
	_sRect.h = 1000;
	_dRect.x = 0;
	_dRect.y = 0;
	_dRect.w = windowW;
	_dRect.h = windowH;

	_textInputs.push_back(new TextInput(_renderer, (windowW / 2) - (175), 100, "player2"));
	_textInputs.push_back(new TextInput(_renderer, (windowW / 2) - (175), 150, "deck index"));
	_textInputs.push_back(new TextInput(_renderer, (windowW / 2) - (175), 200, "ip address"));
	_textInputs.push_back(new TextInput(_renderer, (windowW / 2) - (175), 250, "port"));

	_joinButton = new Button("Join", _renderer, 0, 700);
	_joinButton->setWindowXY((windowW / 2) - (250), 450);

	_backButton = new Button("Back", _renderer, 0, 400);
	_backButton->setWindowXY((windowW / 2) - (250), 570);
}

JoinRoomMenu::~JoinRoomMenu()
{
	delete _joinButton;
	delete _backButton;
	SDL_DestroyTexture(_background);
}

void JoinRoomMenu::handleEvents()
{
	for (TextInput* input : _textInputs)
	{
		if (input->isSelected())
		{
			input->focus();
		}
		else
		{
			input->unfocus();
		}
	}

	if (_joinButton->isSelected()) _buttonSelected = 0;
	else if (_backButton->isSelected()) _buttonSelected = 1;
	else _buttonSelected = -1;


	//if (_playerNameInput->isSelected()) {
	//	_playerNameInput->focus();

	//	_deckSelector->unfocus();
	//	_serverIpInput->unfocus();
	//	_serverPortInput->unfocus();
	//}
	//else if (_deckSelector->isSelected())
	//{
	//	_deckSelector->focus();

	//	_playerNameInput->unfocus();
	//	_serverIpInput->unfocus();
	//	_serverPortInput->unfocus();
	//}
	//else if (_serverIpInput->isSelected())
	//{
	//	_serverIpInput->focus();

	//	_playerNameInput->unfocus();
	//	_deckSelector->unfocus();
	//	_serverPortInput->unfocus();
	//}
	//else if (_serverPortInput->isSelected())
	//{
	//	_serverPortInput->focus();

	//	_playerNameInput->unfocus();
	//	_deckSelector->unfocus();
	//	_serverIpInput->unfocus();
	//}
	//else {
	//	_playerNameInput->unfocus();
	//	_deckSelector->unfocus();
	//	_serverIpInput->unfocus();
	//	_serverPortInput->unfocus();

	//	if (_joinButton->isSelected()) _buttonSelected = 0;
	//	else if (_backButton->isSelected()) _buttonSelected = 1;
	//	else _buttonSelected = -1;
	//}
}

void JoinRoomMenu::handleTextInputEvent(SDL_TextInputEvent textEvent)
{
	std::cout << "handletextinputevent" << std::endl;
	for (TextInput* input : _textInputs)
	{
		if (input->isFocused())
		{
			input->addText(textEvent.text);
			std::cout << "adding text to focused input" << std::endl;
		}
	}

	//if (_playerNameInput->isFocused())
	//{
	//	_playerNameInput->addText(textEvent.text);
	//}
	//else if (_deckSelector->isFocused())
	//{
	//	_deckSelector->addText(textEvent.text);
	//}
	//else if (_serverIpInput->isFocused())
	//{
	//	_serverIpInput->addText(textEvent.text);
	//}
	//else if (_serverPortInput->isFocused()) 
	//{
	//	_serverPortInput->addText(textEvent.text);
	//}
}

void JoinRoomMenu::update(Mouse* mouse)
{
	bool someInputFocused = false;
	for (TextInput* input : _textInputs)
	{
		if (input->isFocused())
		{
			someInputFocused = true;
			input->startTextInput();
		}
		input->update(mouse);
	}
	if (!someInputFocused)
	{
		_textInputs[0]->stopTextInput();
		//could be replaced by SDL_StopTextInput()
		//SDL_Start and StopTextInput() 
	}
	_joinButton->update(mouse);
	_backButton->update(mouse);
 
	//if (_playerNameInput->isFocused())
	//{
	//	_playerNameInput->startTextInput();
	//}
	//else if (_deckSelector->isFocused())
	//{
	//	_deckSelector->startTextInput();
	//}
	//else if (_serverIpInput->isFocused())
	//{
	//	_serverIpInput->startTextInput();
	//}
	//else if (_serverPortInput->isFocused())
	//{
	//	_serverPortInput->startTextInput();
	//}
	//else
	//{
	//	_playerNameInput->stopTextInput();
	//	_deckSelector->stopTextInput();
	//	_serverIpInput->stopTextInput();
	//	_serverPortInput->stopTextInput();
	//}

	//_playerNameInput->update(mouse);
	//_deckSelector->update(mouse);
	//_serverIpInput->update(mouse);
	//_serverPortInput->update(mouse);
}

void JoinRoomMenu::render()
{
	//render background
	SDL_RenderCopy(_renderer, _background, &_sRect, &_dRect);

	for (TextInput* input : _textInputs)
	{
		input->render();
	}
	_joinButton->render();
	_backButton->render();
}

int JoinRoomMenu::menuType()
{
	return _MENU_TYPE;
}

int JoinRoomMenu::getButtonPressed()
{
	return _buttonSelected;
}

void JoinRoomMenu::clearPressedButton()
{
	_buttonSelected = -1;
}

