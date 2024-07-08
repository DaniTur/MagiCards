#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

enum class Color{ RED, GREEN, COLORLESS};

class Card
{
public:
	Card();

	Card(SDL_Renderer* renderer, uint8_t id, std::string name, Color color, uint8_t cost, uint8_t damage, uint8_t defense, const char* texture);

	~Card();

	int getId() const; // testing, remove

	void render(SDL_Rect& destination, float proportion);

private:
	SDL_Renderer* renderer_ = nullptr;
	SDL_Texture* texture_ = nullptr, *textureBack_ = nullptr;
	SDL_Rect sRect_, dRect_;

	bool facedown_ = true;
	uint8_t id_;
	std::string name_;
	Color color_;
	uint8_t cost_;
	uint8_t damage_;
	uint8_t defense_;
};

