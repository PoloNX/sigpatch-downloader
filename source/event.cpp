#include "event.hpp"

eventRequests::eventRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font)
{
	this->window = window;
	this->renderer = renderer;
	this->font = font;
	this->horizontal = horizontal;
	this->vertical = vertical;
}

int eventRequests::eventCursor()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_JOYBUTTONDOWN)
		{
			if (event.jbutton.button == (14 || 12)) //right
			{
				return 1;	//Change value of cursor
			}

			else if (event.jbutton.button == 10) //plus
			{
				SDL_Quit();
			}

			else if (event.jbutton.button == 0) //a button
			{
				return 2;	//Start Download
			}
		}
	}
}