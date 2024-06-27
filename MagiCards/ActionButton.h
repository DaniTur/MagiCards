#pragma once
#include "Button.h"

enum class ActionButtonType {
	OTHER,
	DECK_SHUFFLE,
	DRAW,
	PLAY_CARD
};

class ActionButton : public Button
{
public:
	ActionButton(ActionButtonType type, SDL_Renderer* renderer, SDL_Rect dstRect);

	~ActionButton();

	void changeButtonType(ActionButtonType newType);

	ActionButtonType getType() const;

private:

	ActionButtonType type_;

	const char* buttonTextByType(ActionButtonType type);
};

