#include "Card.h"
#include "ResourcesList.h"
#include <iostream>

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
	textureSelectedFrame_ = IMG_LoadTexture(renderer_, IMG_SELECTED_CARD_FRAME);
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

void Card::update(Mouse* mouse)
{
	if (SDL_HasIntersection(mouse->getTip(), &dRect_))
	{
		std::cout << "mouse hover card: " << unsigned(id_) << std::endl;
		mouseHover_ = true;
	}
	else {
		mouseHover_ = false;
	}
}

void Card::render(SDL_Rect* destination, float proportion)
{
	destination->w = sRect_.w * proportion;
	destination->h = sRect_.h * proportion;

	dRect_.x = destination->x;
	dRect_.y = destination->y;
	dRect_.w = destination->w;
	dRect_.h = destination->h;

	if (facedown_) {
		//SDL_RenderCopy(renderer_, textureBack_.get(), NULL, &destination);
		SDL_RenderCopy(renderer_, textureBack_, NULL, &dRect_);
	}else{
		//SDL_RenderCopy(renderer_, texture_.get(), NULL, &destination);
		SDL_RenderCopy(renderer_, texture_, NULL, &dRect_);
	}

	if (mouseHover_)
	{
		SDL_RenderCopy(renderer_, textureSelectedFrame_, NULL, &dRect_);
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
