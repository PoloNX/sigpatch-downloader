#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

class eventRequests
{
private:
	SDL_Window* window;
	int horizontal;
	int vertical;
	SDL_Renderer* renderer;
	TTF_Font* font;
public:
	eventRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font);
	int eventCursor();
};