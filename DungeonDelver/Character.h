#pragma once
#include "SDL/SDL.h"

struct Circle {
	int x, y; // coordinates
	int radius;
};

// Heirarchy Component - Parent to Snake and Apple
class Character
{
public:
	void RenderCharacter();
	void Line();
private:
	void Draw(SDL_Renderer* renderer);

};
