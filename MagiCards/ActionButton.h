#pragma once
#include "Button.h"

enum class ActionButtonType {
	OTHER,
	DECK_SHUFFLE,
	DRAW,
	PLAY
};

class ActionButton : public Button
{
public:
	ActionButton(ActionButtonType type, SDL_Renderer* renderer, SDL_Rect dstRect);

	~ActionButton();

	void changeButtonType(ActionButtonType newType);

private:

	ActionButtonType type_;

	const char* buttonTextByType(ActionButtonType type);
};

