#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

class menuRequests
{
private:
	SDL_Window* window;
	int horizontal;
	int vertical;
	SDL_Renderer* renderer;
	TTF_Font* font;
public:
	menuRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font);
	void printMenu();
};