#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include <curl/curl.h> 

class CurlRequests 
{

public:
	SDL_Window* window;
	int horizontal;
	int vertical;
	SDL_Renderer* renderer;
	TTF_Font* font;

    CurlRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font);
        
    //string returnDebug();
    bool downloadFile(const char *filename, const char *urlPatches, bool appUpdate);
};
