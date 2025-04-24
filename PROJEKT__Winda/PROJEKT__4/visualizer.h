#pragma once

#include <SDL.h>

struct Button
{
	SDL_Rect rect;
	bool pressed;

};

//jezeli button.pressed == true pojawia sie nowy ludzik na danym pietrze
