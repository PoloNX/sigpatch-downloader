#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include <string>
#include <iostream>
#include <curl/curl.h>

#include "../include/download.hpp"
#include "../include/unzip.hpp"

#define largeur 1280
#define longueur 720
CurlRequests *sessionCurl = new CurlRequests;

int main()
{
    socketInitializeDefault();
    nxlinkStdio();  //redirect cout/cer to tcp
    
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_InitSubSystem(SDL_INIT_EVERYTHING);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);	
    
    romfsInit();
    chdir("romfs:/");

    int langage;
    //langageSettings();    //Later

    TTF_Font* font = TTF_OpenFont("data/arial.ttf", 28);
    TTF_Font* font_big = TTF_OpenFont("data/arial.ttf", 50);
    SDL_Window* mainWindow = SDL_CreateWindow("mainWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largeur, longueur, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);

    SDL_Surface* sigpatch_fusee_s = TTF_RenderText_Blended(font, "Telecharger les Sigpacthes pour fusee_primary.", SDL_Color {255, 255, 255, 255});
    SDL_Texture* sigpatch_fusee = SDL_CreateTextureFromSurface (mainRenderer, sigpatch_fusee_s); 

    SDL_Surface* sigpatch_hekate_s = TTF_RenderText_Blended(font, "Telcharger les Sigpacthes pour Hekate.", SDL_Color {255, 255, 255, 255});
    SDL_Texture* sigpatch_hekate = SDL_CreateTextureFromSurface (mainRenderer, sigpatch_hekate_s);

    SDL_Surface* title_s = TTF_RenderText_Blended(font_big, "Utilitaire de telechargement de sigpatches.", SDL_Color {255, 255, 255, 255});
    SDL_Texture* title = SDL_CreateTextureFromSurface (mainRenderer, title_s); 

    SDL_Rect rect_fusee;	//Rect AMS
    rect_fusee.w = sigpatch_fusee_s->w;
    rect_fusee.h = sigpatch_fusee_s->h;
    rect_fusee.x = largeur / 2 - rect_fusee.w / 2;
    rect_fusee.y = longueur - 400;

    SDL_Rect rect_hekate;	//Rect hekate
    rect_hekate.w = sigpatch_hekate_s->w;
    rect_hekate.h = sigpatch_hekate_s->h;
    rect_hekate.x = largeur / 2 - rect_hekate.w / 2;
    rect_hekate.y = longueur - 200;

    SDL_Rect rect_title;	//Rect title
    rect_title.w = title_s->w;
    rect_title.h = title_s->h;
    rect_title.x = largeur / 2 - rect_title.w / 2;
    rect_title.y = 20;

    SDL_FreeSurface(sigpatch_fusee_s);
    SDL_FreeSurface(sigpatch_hekate_s);
    SDL_FreeSurface(title_s);

    

    bool isOpen = true;		//mainLoop
    int choice = 0;		//Choice for cursor
    SDL_Event event;	//Event
    int background = 0;		//Int for custom background
    chdir("sdmc:/");
    while (isOpen && appletMainLoop())
    {

        SDL_SetRenderDrawColor(mainRenderer, 45, 45, 45, 255);

        if (choice == 2)
        {
            choice = 0;
        }

        if (choice == -1)
        {
            choice = 1;
        }
      
        SDL_RenderClear(mainRenderer);

        while(SDL_PollEvent(&event)){
            if (event.type == SDL_JOYBUTTONDOWN)
            {
                if (event.jbutton.button == 15)	//Flèche du bas
                {
                    ++choice;
                }

                if (event.jbutton.button == 13)	//Flèche du haut
                {
                    --choice;
                }

                if (event.jbutton.button == 0 && choice == 0)   //Button A and cursor on fusee
                {
                    sessionCurl->downloadFile("patch.zip");
                }

                if (event.jbutton.button == 0 && choice == 1)   //Button A and cursor on hekate
                {
                    sessionCurl->downloadFile("patch.zip");
                }
            }
        }

        //render of text
        SDL_RenderCopy(mainRenderer, sigpatch_fusee, NULL, &rect_fusee);
        SDL_RenderCopy(mainRenderer, sigpatch_hekate, NULL, &rect_hekate);
        SDL_RenderCopy(mainRenderer, title, NULL, &rect_title);

        if (choice == 0)
        {
            SDL_SetRenderDrawColor(mainRenderer, 123, 224, 228, 255);
            SDL_RenderDrawRect(mainRenderer, &rect_fusee);
        }

        if (choice == 1)
        {
            SDL_SetRenderDrawColor(mainRenderer, 123, 224, 228, 255);
            SDL_RenderDrawRect(mainRenderer, &rect_hekate);
        }

        SDL_RenderPresent(mainRenderer);	//Render Present

    }

    SDL_Quit();
}
