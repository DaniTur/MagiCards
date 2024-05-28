#pragma once
#include <json/json.h>
#include <json/value.h>
#include <fstream>
#include <iostream>
#include "Card.h"
#include "ResourcesList.h"

class CardFactory
{
public:

	static Card create(int cardId, SDL_Renderer *renderer)
	{
		std::ifstream cardsFile(CARDS_JSON);
		if (!cardsFile.is_open()) {
			std::cout << "Error: Could not open the JSON file." << std::endl;
		}

		Json::Value jsonRoot;
		Json::Reader reader;

		reader.parse(cardsFile, jsonRoot);

		if (!jsonRoot.isArray()) {
			std::cout << "Error: The JSON file should contain an array as the root level." << std::endl;
		}

		int id, cost, damage, defense;
		Color color = Color::COLORLESS;
		std::string name, texture;

		bool found = false;
		int i = 0;
		while (i < jsonRoot.size() && !found) {
			// si es un objeto carda valido el que se ha leido y el id es el que buscamos
			if (isValidCardRead(jsonRoot[i]) && jsonRoot[i]["id"] == cardId)
			{
				//jsonRoot[i] represents a card object(json object)
				id = jsonRoot[i]["id"].asInt();
				name = jsonRoot[i]["name"].asString();
				cost = jsonRoot[i]["cost"].asInt();
				damage = jsonRoot[i]["damage"].asInt();
				defense = jsonRoot[i]["defense"].asInt();
				texture = jsonRoot[i]["texture"].asString();

				switch (jsonRoot[i]["color"].asInt())
				{
				case 0:
					color = Color::RED;
					break;
				case 1:
					color = Color::GREEN;
					break;
				}

				found = true;
			}
			else {
				i++;
			}

		}
		if (found)
		{
			auto texture = getTexture(id);
			return Card(renderer, id, name, color, cost, damage, defense, texture);
		}
		else
		{
			std::cout << "Error loading card." << std::endl;
			std::cout << "Card with ID : " << cardId << "not found in the Json file" << std::endl;
			return Card();
		}
	}

private:
	static bool isValidCardRead(Json::Value cardObject)
	{
		return cardObject.isMember("id") && cardObject.isMember("texture");
	}

	static constexpr const char* getTexture(int id) {
		switch (id)
		{
		case 1:
			return IMG_RED_DRAGON_CARD;
		case 2:
			return IMG_RED_WARRIOR_CARD;
		case 3:
			return IMG_RED_MAGE_CARD;
		case 4:
			return IMG_GREEN_DRAGON_CARD;
		case 5:
			return IMG_GREEN_WARRIOR_CARD;
		case 6:
			return IMG_GREEN_MAGE_CARD;
		default:
			return IMG_REVERSE_CARD;
		}
	}
};