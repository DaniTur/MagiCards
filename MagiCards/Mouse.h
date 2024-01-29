#pragma once
#include <SDL.h>

class Mouse
{
public:
	Mouse(SDL_Renderer* renderer);
	~Mouse();

	void render();

	SDL_Rect* getCursor();

	SDL_Rect* getTip();

	void setTipXY(int x, int y);

private:
	SDL_Texture* _texture;
	SDL_Rect _cursor, _tip; //tip: mouse clickable pixel point
	SDL_Renderer* _renderer;
};

