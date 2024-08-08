#include "Card.h"
#include <iostream>

Card::Card()
{
}

Card::Card(SDL_Renderer* renderer, uint8_t id, std::string_view name, Color color, uint8_t cost, uint8_t damage, uint8_t defense, std::string_view texture)
	: renderer_(renderer), texturePath_(texture)
{
	id_ = id;
	name_ = name;
	color_ = color;
	cost_ = cost;
	damage_ = damage;
	defense_ = defense;

	textureBack_ = IMG_LoadTexture(renderer_, IMG_REVERSE_CARD);
	texture_ = IMG_LoadTexture(renderer_, texturePath_.c_str());
	textureSelectedFrame_ = IMG_LoadTexture(renderer_, IMG_SELECTED_CARD_FRAME);

	textFont_ = TTF_OpenFont(TEXT_FONT, 14);

	//textureBack_ = std::unique_ptr<SDL_Texture, TextureDestructor>
	//	(IMG_LoadTexture(renderer_, IMG_REVERSE_CARD)
		//, SDL_DestroyTexture);

	//card texture source size
	sRect_.w = 215;
	sRect_.h = 300;
	sRect_.x = 2;

	dRect_ = { 0, 0, 0, 0 };
}

// Copy constructor (performs a Deep copy)
Card::Card(const Card& card)
	: renderer_(card.renderer_), sRect_(card.sRect_), dRect_(card.dRect_), texturePath_(card.texturePath_),
	mouseHover_(card.mouseHover_), selected_(card.selected_),
	facedown_(card.facedown_), id_(card.id_), name_(card.name_),
	color_(card.color_), cost_(card.cost_), damage_(card.damage_),
	defense_(card.defense_){

	texture_ = IMG_LoadTexture(renderer_, texturePath_.c_str());
	textureBack_ = IMG_LoadTexture(renderer_, IMG_REVERSE_CARD);
	textureSelectedFrame_ = IMG_LoadTexture(renderer_, IMG_SELECTED_CARD_FRAME);
	textFont_ = TTF_OpenFont(TEXT_FONT, 12);
}

Card::~Card()
{
	SDL_DestroyTexture(texture_);
	SDL_DestroyTexture(textureBack_);
	SDL_DestroyTexture(textureSelectedFrame_);
}

int Card::getId() const
{
	return static_cast<int>(id_);
}

void Card::update(Mouse* mouse)
{
	if (SDL_HasIntersection(mouse->getTip(), &dRect_))
	{
		mouseHover_ = true;
	}
	else {
		mouseHover_ = false;
	}
}

void Card::render(SDL_Rect* destination, float proportion)
{
	destination->w = static_cast<int>(sRect_.w * proportion);
	destination->h = static_cast<int>(sRect_.h * proportion);

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

		// Render Card data on top
		renderCardTextData();
	}

	if (mouseHover_ || selected_)
	{
		SDL_RenderCopy(renderer_, textureSelectedFrame_, nullptr, &dRect_);
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

bool Card::isMouseHovered() const
{
	return mouseHover_;
}

bool Card::isSelected() const
{
	return selected_;
}

void Card::select()
{
	selected_ = true;
}

void Card::deselect()
{
	selected_ = false;
}

void Card::renderCardTextData()
{
	// Render Card Name
	SDL_Surface* textSurface = TTF_RenderText_Solid(textFont_, name_.c_str(), { 0, 0, 0, 255 });
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_, textSurface);
	
	SDL_Rect textDRect;
	textDRect.x = dRect_.x + 5; //top left margin
	textDRect.y = dRect_.y + 5;
	textDRect.w = dRect_.w - 8;
	textDRect.h = textSurface->h -5;

	SDL_RenderCopy(renderer_, textTexture, nullptr, &textDRect);
	
	// Render Card damage/defense
	std::string text = std::to_string(damage_) + "/" + std::to_string(defense_);
	textSurface = TTF_RenderText_Solid(textFont_, text.c_str(), { 0, 0, 0, 255 });
	textTexture = SDL_CreateTextureFromSurface(renderer_, textSurface);

	textDRect.x = dRect_.x + dRect_.w - textSurface->w - 10; // x card start + card width - text size - margin
	textDRect.y = dRect_.y + dRect_.h - textSurface->h - 10;
	textDRect.w = textSurface->w;
	textDRect.h = textSurface->h;

	SDL_RenderCopy(renderer_, textTexture, nullptr, &textDRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}
