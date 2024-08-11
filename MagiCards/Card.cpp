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

// Constructor de Movimiento
Card::Card(Card&& card) noexcept
	: renderer_(card.renderer_), texture_(card.texture_), textureBack_(card.textureBack_),
	textureSelectedFrame_(card.textureSelectedFrame_), sRect_(card.sRect_), dRect_(card.dRect_),
	texturePath_(std::move(card.texturePath_)), textFont_(card.textFont_), 
	mouseHover_(card.mouseHover_),  selected_(card.selected_), facedown_(card.facedown_), 
	id_(card.id_), name_(std::move(card.name_)), color_(card.color_), cost_(card.cost_),
	damage_(card.damage_), defense_(card.defense_)  {

	// Transferir la propiedad de los recursos del objeto `card`
	card.texture_ = nullptr;
	card.textureBack_ = nullptr;
	card.textureSelectedFrame_ = nullptr;
	card.textFont_ = nullptr;

	// Opcional: Si los recursos deben ser inicializados o reiniciados, hazlo aquí
	//texture_ = IMG_LoadTexture(renderer_, texturePath_.c_str());
	//textureBack_ = IMG_LoadTexture(renderer_, IMG_REVERSE_CARD);
	//textureSelectedFrame_ = IMG_LoadTexture(renderer_, IMG_SELECTED_CARD_FRAME);
	//textFont_ = TTF_OpenFont(TEXT_FONT, 12);
}

// Operador de Asignación de Copia
Card& Card::operator=(const Card& other) {
	if (this != &other) {
		// Primero libera los recursos actuales
		if (texture_) {
			SDL_DestroyTexture(texture_);
			texture_ = nullptr;  // Opcional: asegurar que el puntero esté en un estado válido
		}
		if (textureBack_) {
			SDL_DestroyTexture(textureBack_);
			textureBack_ = nullptr;  // Opcional
		}
		if (textureSelectedFrame_) {
			SDL_DestroyTexture(textureSelectedFrame_);
			textureSelectedFrame_ = nullptr;  // Opcional
		}
		if (textFont_) {
			TTF_CloseFont(textFont_);
			textFont_ = nullptr;  // Opcional
		}

		// Luego copia los recursos del objeto 'other'
		renderer_ = other.renderer_;
		sRect_ = other.sRect_;
		dRect_ = other.dRect_;
		texturePath_ = other.texturePath_;
		mouseHover_ = other.mouseHover_;
		selected_ = other.selected_;
		facedown_ = other.facedown_;
		id_ = other.id_;
		name_ = other.name_;
		color_ = other.color_;
		cost_ = other.cost_;
		damage_ = other.damage_;
		defense_ = other.defense_;

		// Asignar nuevas instancias de recursos
		texture_ = IMG_LoadTexture(renderer_, texturePath_.c_str());
		textureBack_ = IMG_LoadTexture(renderer_, IMG_REVERSE_CARD);
		textureSelectedFrame_ = IMG_LoadTexture(renderer_, IMG_SELECTED_CARD_FRAME);
		textFont_ = TTF_OpenFont(TEXT_FONT, 12);
	}
	return *this;
}

Card::~Card()
{
	// Solo liberar recursos si no son nullptr
	if (texture_) SDL_DestroyTexture(texture_);
	if (textureBack_) SDL_DestroyTexture(textureBack_);
	if (textureSelectedFrame_) SDL_DestroyTexture(textureSelectedFrame_);
	if (textFont_) TTF_CloseFont(textFont_);
}

int Card::getId() const
{
	return static_cast<int>(id_);
}

int Card::getDamage() const
{
	return damage_;
}

int Card::getDefense() const
{
	return defense_;
}

int Card::dealDamage(int damage)
{
	int overfloatDamage = defense_ - damage;
	defense_ -= damage;

	// Si hay daño excedente que no cubre la defensa de la carta, devuelve daño excedente
	if (overfloatDamage < 0)
	{
		return (overfloatDamage * -1); // devovler el daño excedente como valor positivo
	}
	else {	// Si no hay daño excedente porque la defensa de la carta cubre el ataque, devolver cero de daño excedente
		return 0;
	}

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
