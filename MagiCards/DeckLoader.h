#pragma once
#include <json/json.h>
#include <json/value.h>
#include <fstream>
#include <iostream>
#include "Deck.h"
#include "ResourcesList.h"
#include "CardFactory.h"

class DeckLoader
{
public:
	// Fills the deck passed as reference with the cards from the deck.json with deckId as identifier
	static void load(uint8_t deckId, std::vector<Card>& deck, SDL_Renderer* renderer)
	{
		std::ifstream decksFile(DECKS_JSON);
		if (!decksFile.is_open()) {
			std::cout << "Error: Could not open the JSON file." << std::endl;
		}

		Json::Value jsonRoot;
		Json::Reader reader;

		reader.parse(decksFile, jsonRoot);

		if (!jsonRoot.isArray()) {
			std::cout << "Error: The JSON file should contain an array as the root level." << std::endl;
		}

		// Find if the deck to be loaded exists in the json file
		bool deckFound = false;
		int i = 0;
		while (i < jsonRoot.size() && !deckFound)
		{
			if (!jsonRoot[i].isMember("deck_id") || !jsonRoot[i].isMember("cards")) 
			{
				std::cout << "Error: Deck read has wrong deck attributes('deck_id' or 'cards' not found)" << std::endl;
			}
			else
			{
				if (jsonRoot[i]["deck_id"].asInt() == deckId) deckFound = true;
				else i++;
			}
		}

		if (deckFound)
		{
			Json::Value deckObj = jsonRoot[i];
			int id = deckObj["deck_id"].asInt();
			const Json::Value& cardsObj = deckObj["cards"];

			if (cardsObj.isArray())
			{ 
				for (const auto& cardObj : cardsObj)
				{
					// Check if the object read has the base attributes to be considered a valid card to be loaded
					if (cardObj.isMember("card_id") && cardObj.isMember("units"))
					{
						int cardId = cardObj["card_id"].asInt();
						int units = cardObj["units"].asInt();

						// Use a cardFactory to create cards based on its cardId
						Card card = CardFactory::create(cardId, renderer);
						for (int i = 0; i < units; i++)
						{
							//deck.cards.push_back(card);
							deck.push_back(card);
						}
					}
					else
					{
						std::cout << "Error: Card read has wrong card attributes('card_id' or 'units' not found)" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "Error: 'cards' attribute is not an array, of deck with id: " << deckId << std::endl;
			}
		}
		else
		{
			std::cout << "Error: deck with id: " << deckId << " not found in the decks.json" << std::endl;
		}
	}

	static void load(std::vector<int>& cardIDs, std::vector<Card>& deck, SDL_Renderer* renderer)
	{
		for (int cardId : cardIDs)
		{
			Card card = CardFactory::create(cardId, renderer);
			deck.push_back(card);
		}
	}
};