#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>
#include <functional>
#include "Mouse.h"

enum class Color{ RED, GREEN, COLORLESS};

class Card
{
public:
	Card();

	Card(SDL_Renderer* renderer, uint8_t id, std::string name, Color color, uint8_t cost, uint8_t damage, uint8_t defense, const char* texture);

	~Card();

	int getId() const;

	void update(Mouse* mouse);

	void render(SDL_Rect* destination, float proportion);

	void turnUp();

	void turnDown();

private:
	SDL_Renderer* renderer_ = nullptr;

	//using TextureDestructor = std::function<void(SDL_Texture*)>;
	//std::unique_ptr<SDL_Texture, TextureDestructor> texture_;
	//
	//using TextureDestructor = std::function<void(SDL_Texture*)>;
	//std::unique_ptr<SDL_Texture, TextureDestructor> textureBack_;

	SDL_Texture *texture_ = nullptr, *textureBack_ = nullptr, *textureSelectedFrame_ = nullptr;
	SDL_Rect sRect_, dRect_;

	bool mouseHover_ = false;

	bool facedown_ = true;
	Uint8 id_;
	std::string name_;
	Color color_;
	Uint8 cost_;
	Uint8 damage_;
	Uint8 defense_;
};

