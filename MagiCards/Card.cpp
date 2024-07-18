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

	//texture_ = std::unique_ptr<SDL_Texture, TextureDestructor>
	//	(IMG_LoadTexture(renderer_, texture)
	//	, SDL_DestroyTexture);

	//textureBack_ = std::unique_ptr<SDL_Texture, TextureDestructor>
	//	(IMG_LoadTexture(renderer_, IMG_REVERSE_CARD)
		//, SDL_DestroyTexture);

	//card texture source size
	sRect_.w = 215;
	sRect_.h = 300;
	sRect_.x = 2;

	dRect_ = { 0, 0, 0, 0 };
}

Card::~Card()
{
	//SDL_DestroyTexture(texture_);
}

int Card::getId() const
{
	return static_cast<int>(id_);
}

void Card::render(SDL_Rect* destination, float proportion)
{
	destination->w = sRect_.w * proportion;
	destination->h = sRect_.h * proportion;

	if (facedown_) {
		//SDL_RenderCopy(renderer_, textureBack_.get(), NULL, &destination);
		SDL_RenderCopy(renderer_, textureBack_, NULL, destination);
	}else{
		//SDL_RenderCopy(renderer_, texture_.get(), NULL, &destination);
		SDL_RenderCopy(renderer_, texture_, NULL, destination);
	}
}

void Card::turnUp()
{
	facedown_ = false;
}

void Card::turnDown()
{
	facedown_ = true;
}
