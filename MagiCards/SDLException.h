#pragma once

#include <iostream>

class SDLException : public std::exception
{
public:

	SDLException(const char *message) : _message(message){}

	virtual char const* what() const { return _message; }

	const char* _message;
};

