#include <SDL_image.h>
#include "JoinRoomMenu.h"
#include "ResourcesList.h"

JoinRoomMenu::JoinRoomMenu()
{
}

JoinRoomMenu::JoinRoomMenu(SDL_Renderer* renderer) : _renderer(renderer)
{
	const int windowW = 1280;
	const int windowH = 720;

    _background = IMG_LoadTexture(_renderer, IMG_JOIN_ROOM_MENU);

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
}

void JoinRoomMenu::handleTextInputEvent(SDL_TextInputEvent textEvent)
{
	if (isValidText(textEvent.text[0]))
	{
		for (TextInput* input : _textInputs)
		{
			if (input->isFocused())
			{
				input->addText(textEvent.text);
			}
		}
	}
}

bool JoinRoomMenu::isValidText(const char text) {
	//check if character is not ASCII (non english characters)
	if (static_cast<unsigned char>(text) > 127) return false;

	if (!isdigit(text) && !isalpha(text) 
		&& (static_cast<unsigned char>(text) != 46)) return false;

	return true;
}

void JoinRoomMenu::handleKeyDownEvent(SDL_Keysym keysym) 
{
	for (TextInput* input : _textInputs)
	{
		if (input->isFocused() && keysym.sym == SDLK_BACKSPACE) //delete button
		{
			input->deleteChar();
		}
	}
}

std::string JoinRoomMenu::getPlayerName()
{
	return _textInputs[PLAYER_NAME]->getText();
}

int JoinRoomMenu::getSelectedDeck()
{
	return std::stoi(_textInputs[DECK_SELECTOR]->getText());
}

std::string JoinRoomMenu::getServerAddress()
{
	return _textInputs[SERVER_IP]->getText();
}

int JoinRoomMenu::getServerPort()
{
	return stoi(_textInputs[SERVER_PORT]->getText());
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

