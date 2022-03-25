#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
#pragma once

class utilsRequests
{
private:
	SDL_Window* window;
	int horizontal;
	int vertical;
	SDL_Renderer* renderer;
	TTF_Font* font;
public:
	utilsRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font);
	SDL_Rect printStringCenter(std::string text, int addToX, int addToY, bool printRect, int fontSize, SDL_Color color);
	void LoadImage(std::string file, int x, int y);
	void printProgressBar(int dlNow, int dlTotal, int dlSpeed);
};