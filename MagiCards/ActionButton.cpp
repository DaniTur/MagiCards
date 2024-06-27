#include "ActionButton.h"

ActionButton::ActionButton(ActionButtonType type, SDL_Renderer* renderer, SDL_Rect dstRect) 
	: Button(buttonTextByType(type), renderer, {0, 800}, dstRect), type_(type) // {0, 800} SDL_Rect source from texture
{
}

ActionButton::~ActionButton()
{
}

void ActionButton::changeButtonType(ActionButtonType newType)
{
	type_ = newType;
	changeText(buttonTextByType(type_));
}

ActionButtonType ActionButton::getType() const
{
	return type_;
}

const char* ActionButton::buttonTextByType(ActionButtonType type)
{
	switch (type)
	{
	case ActionButtonType::DECK_SHUFFLE:
		return "SHUFFLE";

	case ActionButtonType::DRAW:
		return "DRAW";

	case ActionButtonType::PLAY_CARD:
		return "PLAY";

	default:
		return "";
	}
}