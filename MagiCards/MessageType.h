#pragma once
#include <cstdint>

enum class MessageType : uint32_t
{
	WelcomeClient,
	Error,
	PlayerData
};