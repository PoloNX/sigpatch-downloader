#include "utils.hpp"

utilsRequests::utilsRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font)
{
	this->window = window;
	this->renderer = renderer;
	this->font = font;
	this->horizontal = horizontal;
	this->vertical = vertical;
}

SDL_Rect utilsRequests::printStringCenter(std::string text, int addToX, int addToY, bool printRect, int fontSize, SDL_Color color)
{
	std::cout << "dans utils\n";
	TTF_Font* fontS = TTF_OpenFont("data/BerlinSansFB.ttf", fontSize);
	std::cout << "avant surface\n";
	SDL_Surface* surface; 
	std::cout << "avant font\n";
	surface = TTF_RenderText_Blended(fontS, text.c_str(), color);
	std::cout << "après surface\n";
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int t_w;
	int t_h;
	SDL_QueryTexture(texture, nullptr, nullptr, &t_w, &t_h);
	SDL_Rect rect{ (horizontal / 2 - t_w / 2) + addToX, (vertical / 2 - t_h / 2) + addToY, t_w, t_h };
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
	SDL_Rect rect1{ (horizontal / 2 - t_w / 2) + addToX - 5, (vertical / 2 - t_h / 2) + addToY - 5, t_w + 10, t_h + 10 };
	if (printRect)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &rect1);
	}
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	std::cout << "avant return\n";
	return rect1;
}

void utilsRequests::LoadImage(std::string file, int x, int y)
{
	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(renderer, file.c_str());
	if (texture == nullptr)
	{
		std::cout << "Failed to load image: " + file + IMG_GetError();
		return;
	}
	int t_w;
	int t_h;
	SDL_QueryTexture(texture, nullptr, nullptr, &t_w, &t_h);
	SDL_Rect rect{ x, y, t_w, t_h };
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
	SDL_DestroyTexture(texture);
}

void utilsRequests::printProgressBar(int dlNow, int dlTotal, int dlSpeed)
{
	SDL_SetRenderDrawColor(renderer, 30, 29, 29, 255);
	SDL_Rect rectTotal {-horizontal/100*25, vertical/100*50-25, horizontal/2, 50};

	SDL_SetRenderDrawColor(renderer, 123, 224, 228, 255);
	SDL_Rect rectNow {-horizontal/100*25, vertical/100*50-25, (horizontal/2)/100*(dlTotal/dlNow), 50};

	printStringCenter((std::to_string(dlNow) + "mB/" + std::to_string(dlTotal) + "mB"), 0, 30, false, 30, SDL_Color{255, 255, 255, 255});
	printStringCenter((std::to_string(dlTotal/dlNow) + "%"), horizontal/100*27, 0, false, 30, SDL_Color{255, 255, 255, 255});

	SDL_RenderPresent(renderer);
}