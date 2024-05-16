#pragma once
#include <cstdint>

enum class MessageType : uint32_t
{
	ServerAccept,
	JoinRoom,
	ServerError,
	PlayerData
};