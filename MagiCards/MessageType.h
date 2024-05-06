#pragma once
#include <cstdint>

enum class MessageType : uint32_t
{
	ServerAccept,
	WelcomeClient,
	Error,
	PlayerData
};