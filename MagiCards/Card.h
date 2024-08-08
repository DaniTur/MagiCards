#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>
#include <functional>
#include <SDL_ttf.h>
#include "Mouse.h"
#include "ResourcesList.h"

enum class Color{ RED, GREEN, COLORLESS};

class Card
{
public:
	Card();

	Card(SDL_Renderer* renderer, uint8_t id, std::string_view name, Color color, uint8_t cost, uint8_t damage, uint8_t defense, std::string_view texture);

	// Copy constructor (performs a Deep copy)
	Card(const Card& card);

	~Card();

	int getId() const;

	void update(Mouse* mouse);

	void render(SDL_Rect* destination, float proportion);

	void turnUp();

	void turnDown();

	bool isMouseHovered() const;

	bool isSelected() const;

	void select();

	void deselect();

private:

	void renderCardTextData();

private:
	SDL_Renderer* renderer_ = nullptr;

	//using TextureDestructor = std::function<void(SDL_Texture*)>;
	//std::unique_ptr<SDL_Texture, TextureDestructor> textureSafe_;
	//
	//using TextureDestructor = std::function<void(SDL_Texture*)>;
	//std::unique_ptr<SDL_Texture, TextureDestructor> textureBackSafe_;

	SDL_Texture* texture_ = nullptr;
	SDL_Texture* textureBack_ = nullptr;
	SDL_Texture* textureSelectedFrame_ = nullptr;
	SDL_Rect sRect_;
	SDL_Rect dRect_;
	std::string texturePath_;
	
	TTF_Font* textFont_ = nullptr;

	bool mouseHover_ = false;
	bool selected_ = false;

	bool facedown_ = true;
	Uint8 id_;
	std::string name_;
	Color color_;
	Uint8 cost_;
	Uint8 damage_;
	Uint8 defense_;
};

