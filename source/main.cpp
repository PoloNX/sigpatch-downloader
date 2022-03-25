#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <switch.h>

#include "renderer.hpp"

const int horizontal = 1280;
const int vertical = 720;
bool exit_requested = false;		//mainLoop

int main()
{
    socketInitializeDefault();
    nxlinkStdio();  //redirect cout/cer to tcp
    
    SDL_Init(SDL_INIT_VIDEO);  //Init for SDL2
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); //Init for SDL_ttf
    SDL_InitSubSystem(SDL_INIT_EVERYTHING); //Some init 
    SDL_JoystickEventState(SDL_ENABLE); //Joystick
    SDL_JoystickOpen(0);    //Open joystick
    
    romfsInit();    //Init of romfs
    chdir("romfs:/");   //go on romfs

    TTF_Font* font = TTF_OpenFont("data/BerlinSansFB.ttf", 30);    //font for text

    SDL_Window* pWindow = SDL_CreateWindow("mainWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, horizontal, vertical, SDL_WINDOW_RESIZABLE);   //The window
    SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE); //The renderer

    chdir("sdmc:/");    //Go in the sdmc

    while (!exit_requested/* && appletMainLoop()*/)
    {
        menuRequests menu(pWindow, horizontal, vertical, pRenderer, font);
        menu.printMenu();
        SDL_RenderPresent(pRenderer); //Render Present
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit(); //Quit
    romfsExit();
    socketExit();
    return EXIT_SUCCESS;
}
