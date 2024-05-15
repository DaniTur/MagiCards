#pragma once
#include <iostream>
#include <vector>
#include "MessageType.h"

struct MessageHeader
{
	MessageType id;
	uint32_t size = sizeof(uint32_t) + sizeof(MessageType); //whole message size
};

struct Message
{
	MessageHeader header{};

	// using uint8_t makes it work in bytes
	std::vector<uint8_t> body;

	size_t size() const
	{
		return sizeof(MessageHeader) + body.size();
	}

	// override << operator for messages output
	friend std::ostream& operator << (std::ostream& os, const Message& message) 
	{
		os << "ID:" << int(message.header.id) << " Size:" << message.header.size;
		return os;
	}

	// override << operator for adding data to a message
	template<typename DataType>
	friend Message& operator << (Message& message, const DataType& data)
	{
		// If the data type does not have a standard memory layout, generate an error
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t previousBodySize = message.body.size();

		message.body.resize(previousBodySize + sizeof(DataType));

		// Copy the data into the memory space we resized
		// starting message body data memory location + the previous body size is where the new data needs to be added
		std::memcpy(message.body.data() + previousBodySize, &data, sizeof(DataType));

		message.header.size = message.size(); //new size updated

		return message;
	}

	template<typename DataType>
	friend Message& operator >> (Message& message, DataType& data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t newBodySize = message.body.size() - sizeof(DataType);

		std::memcpy(&data, message.body.data() + newBodySize, sizeof(DataType));

		message.body.resize(newBodySize);

		return message;
	}
};