#pragma once
#include "Mouse.h"

class Menu //Interface
{
public:
	virtual void handleEvents() = 0;

	virtual void update(Mouse* mouse) = 0;

	virtual void render() = 0;

	virtual int menuType() = 0;

	virtual int getButtonPressed() = 0;

	virtual void clearPressedButton() = 0;
};

