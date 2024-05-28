#include "Card.h"
#include "ResourcesList.h"

Card::Card()
{
}

Card::Card(SDL_Renderer* renderer, uint8_t id, std::string name, Color color, uint8_t cost, uint8_t damage, uint8_t defense, const char* texture)
	: renderer_(renderer)
{
	id_ = id;
	name_ = name;
	color_ = color;
	cost_ = cost;
	damage_ = damage;
	defense_ = defense;

	textureBack_ = IMG_LoadTexture(renderer_, IMG_REVERSE_CARD);
	texture_ = IMG_LoadTexture(renderer_, texture);
}

Card::~Card()
{
	SDL_DestroyTexture(texture_);
}
