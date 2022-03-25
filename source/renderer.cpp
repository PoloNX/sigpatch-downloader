#include "renderer.hpp"
#include "utils.hpp"
#include "event.hpp"
#include "download.hpp"
#include "unzip.hpp"

menuRequests::menuRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font)
{
	this->window = window;
	this->renderer = renderer;
	this->font = font;
	this->horizontal = horizontal;
	this->vertical = vertical;
}

SDL_Color white{ 255, 255, 255, 255 };
SDL_Color red{ 255, 0, 0, 255, };

int weightOfString(std::string text)
{
	TTF_Font* fontS = TTF_OpenFont("C:\\Windows\\fonts\\arial.ttf", 30);
	SDL_Surface* surface = TTF_RenderText_Blended(fontS, text.c_str(), SDL_Color{ 255, 255, 255, 255 });
	return surface->w;
}

int heightOfString(std::string text)
{
	TTF_Font* fontS = TTF_OpenFont("C:\\Windows\\fonts\\arial.ttf", 30);
	SDL_Surface* surface = TTF_RenderText_Blended(fontS, text.c_str(), SDL_Color{ 255, 255, 255, 255 });
	return surface->h;
}


void menuRequests::printMenu()
{
	utilsRequests utils(window, horizontal, vertical, renderer, font);



	SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);  //Render Color (grey)
	SDL_RenderClear(renderer);

	std::cout << "1\n";

	//Print Text
	utils.printStringCenter("SigPatch Downloader", 0, -vertical / 100 * 30, false, 45, white);

	std::cout << "2\n";

	utils.printStringCenter("SigPatch Downloader", 0, -vertical / 100 * 20, false, 30, white);
	utils.printStringCenter("Update App", horizontal / 100 * 20, vertical / 100 * 20, false, 30, white);
	utils.printStringCenter("Update SigPatches", -horizontal / 100 * 20, vertical / 100 * 20, false, 30, white);
	utils.printStringCenter("By PoloNX", 0, vertical / 100 * 45, false, 15, red);

	//Load Image
	utils.LoadImage("data/fusee.png", -horizontal / 100 * 20, -vertical / 100 * 5);
	utils.LoadImage("data/download.png", horizontal / 100 * 20, -vertical / 100 * 5);

	std::cout << "après utils\n";

	int cursor = 0;
	int xCursor = -horizontal / 100 * 22;
	int wCursor = 0;

	//Print Cursor
	if (cursor == 0) {
		xCursor = -horizontal / 100 * 22;
		wCursor = weightOfString("Update SigPatches") + horizontal / 100 * 4;
	}

	else if (cursor == 1) {
		xCursor = horizontal / 100 * 18;
		wCursor = weightOfString("Update App") + horizontal / 100 * 4;
	}

	SDL_Rect rectCursor { xCursor, vertical / 100 * 18, wCursor, heightOfString("Update") + 4 };
	SDL_SetRenderDrawColor(renderer, 123, 224, 228, 255);
	SDL_RenderDrawRect(renderer, &rectCursor);

	std::cout << "avant event\n";

	//Check Event
	eventRequests event(window, horizontal, vertical, renderer, font);
	int eventValue = event.eventCursor();
	if (eventValue == 1) {
		if (cursor == 0)
			++cursor;
		else if (cursor == 1)
			--cursor;
	}
	if (eventValue == 2) {
		CurlRequests download(window, horizontal, vertical, renderer, font);
		if (cursor == 0){
			download.downloadFile("patch.zip", "https://github.com/ITotalJustice/patches/releases/latest/download/SigPatches.zip", false);
			unzipRequests unzip;
			unzip.unzipPatches();
		}
		else if (cursor == 1){
			download.downloadFile("patch.zip", "https://github.com/PoloNX/sigpatch-downloader/releases/latest/download/sigpatch-downloader.nro", true);
		}
	}
	
	std::cout << "après check download\n";

	SDL_RenderPresent(renderer);

}