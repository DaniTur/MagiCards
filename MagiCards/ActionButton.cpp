#include "ActionButton.h"
#include <iostream>

ActionButton::ActionButton(ActionButtonType type, SDL_Renderer* renderer, SDL_Rect dstRect) 
	: Button(buttonTextByType(type), renderer, {0, 800}, dstRect), type_(type) // {0, 800} SDL_Rect source from texture
{
}

ActionButton::~ActionButton() = default;

void ActionButton::changeButtonType(ActionButtonType newType)
{
	if (type_ != newType)
	{
		type_ = newType;
		changeText(buttonTextByType(type_));
	}
}

ActionButtonType ActionButton::getType() const
{
	return type_;
}

const char* ActionButton::buttonTextByType(ActionButtonType type) const
{
	switch (type)
	{
	case ActionButtonType::DECK_SHUFFLE:
		return "SHUFFLE";

	case ActionButtonType::DRAW:
		return "DRAW";

	case ActionButtonType::PLAY_CARD:
		return "PLAY";

	case ActionButtonType::END_TURN:
		return "END TURN";

	case ActionButtonType::INACTIVE:
		return "INACTIVE";

	default:
		return "";
	}
}