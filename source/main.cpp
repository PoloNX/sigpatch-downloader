#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_image.h>
#include <switch.h>
#include <string>
#include <iostream>
#include <curl/curl.h>

#include "../Include/download.hpp"
#include "../Include/unzip.hpp"
#include "../Include/reboot.h"

#define largeur 1280
#define longueur 720
int main()
{
    socketInitializeDefault();
    nxlinkStdio();  //redirect cout/cer to tcp
    
    SDL_Init(SDL_INIT_EVERYTHING);  //Init for SDL2
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); //Init for SDL_ttf
    SDL_InitSubSystem(SDL_INIT_EVERYTHING); //Some init 
    SDL_JoystickEventState(SDL_ENABLE); //Joystick
    SDL_JoystickOpen(0);    //Open joystick
    
    romfsInit();    //Init of romfs
    chdir("romfs:/");   //go on romfs

    TTF_Font* font_small = TTF_OpenFont("data/BerlinSansFB.ttf", 24);
    TTF_Font* font = TTF_OpenFont("data/BerlinSansFB.ttf", 48);    //font for text
    TTF_Font* font_big = TTF_OpenFont("data/BerlinSansFB.ttf", 72);    //font for title

    SDL_Window* mainWindow = SDL_CreateWindow("mainWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largeur, longueur, SDL_WINDOW_RESIZABLE);   //The window
    SDL_Renderer* mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_SOFTWARE); //The renderer

    SDL_Surface* sigpatch_fusee_s = TTF_RenderText_Blended(font, "Fusee-Primary", SDL_Color {255, 255, 255, 255}); //We create the option for fusee_primary
    SDL_Texture* sigpatch_fusee = SDL_CreateTextureFromSurface (mainRenderer, sigpatch_fusee_s); //We convert the surface to texture

    SDL_Surface* updateApp_s = TTF_RenderText_Blended(font, "Update App", SDL_Color {255, 255, 255, 255}); //We create the option for fusee_primary
    SDL_Texture* updateApp = SDL_CreateTextureFromSurface (mainRenderer, updateApp_s); //We convert the surface to texture

    SDL_Surface* sigpatch_hekate_s = TTF_RenderText_Blended(font, "Hekate", SDL_Color {255, 255, 255, 255}); //We create the option for hekate
    SDL_Texture* sigpatch_hekate = SDL_CreateTextureFromSurface (mainRenderer, sigpatch_hekate_s); //We convert the surface to texture

    SDL_Surface* title_s = TTF_RenderText_Blended(font_big, "SigPatch Downloader", SDL_Color {255, 255, 255, 255}); //We create the title
    SDL_Texture* title = SDL_CreateTextureFromSurface (mainRenderer, title_s); //We convert the surface to texture

    SDL_Surface* description_s = TTF_RenderText_Blended(font, "Download sigpatch on your Switch.", SDL_Color {255, 255, 255, 255}); //We create the option for hekate
    SDL_Texture* description = SDL_CreateTextureFromSurface (mainRenderer, description_s); //We convert the surface to texture   

    SDL_Surface* credit_s = TTF_RenderText_Blended(font_small, "By PoloNX", SDL_Color {255, 255, 255, 255}); //We create the option for hekate
    SDL_Texture* credit = SDL_CreateTextureFromSurface (mainRenderer, credit_s); //We convert the surface to texture 

    SDL_Surface* download_s = TTF_RenderText_Blended(font, "Download starting...", SDL_Color {255, 0, 0, 255}); //We create the title
    SDL_Texture* download = SDL_CreateTextureFromSurface (mainRenderer, download_s); //We convert the surface to texture

    SDL_Surface* downloadEnd_s = TTF_RenderText_Blended(font, "Download finish. please reboot the Nintendo Switch", SDL_Color {0, 0, 255, 255}); //We create the title
    SDL_Texture* downloadEnd = SDL_CreateTextureFromSurface (mainRenderer, downloadEnd_s); //We convert the surface to texture

    SDL_Surface* amsLogo_s = IMG_Load("data/fusee.png");
    SDL_Texture* amsLogo = SDL_CreateTextureFromSurface(mainRenderer, amsLogo_s);

    SDL_Surface* hekateLogo_s = IMG_Load("data/hekate.png");
    SDL_Texture* hekateLogo = SDL_CreateTextureFromSurface(mainRenderer, hekateLogo_s); 

    SDL_Rect rect_cursorAms;
    rect_cursorAms.w = sigpatch_fusee_s->w + 50;
    rect_cursorAms.h = sigpatch_fusee_s->h + 20;
    rect_cursorAms.x = 75;
    rect_cursorAms.y = longueur - 210;

    SDL_Rect rect_cursorHekate;
    rect_cursorHekate.w = sigpatch_fusee_s->w + 50;
    rect_cursorHekate.h = sigpatch_fusee_s->h + 20;
    rect_cursorHekate.x = 860;
    rect_cursorHekate.y = longueur - 210;

    SDL_Rect rect_amsLogo;
    rect_amsLogo.w = amsLogo_s->w;
    rect_amsLogo.h = amsLogo_s->h;
    rect_amsLogo.x = 0;
    rect_amsLogo.y = 150;

    SDL_Rect rect_hekateLogo;
    rect_hekateLogo.w = hekateLogo_s->w;
    rect_hekateLogo.h = hekateLogo_s->h;
    rect_hekateLogo.x = 795;
    rect_hekateLogo.y = 150;
    
    SDL_Rect rect_fusee;	//Rect AMS
    rect_fusee.w = sigpatch_fusee_s->w;
    rect_fusee.h = sigpatch_fusee_s->h;
    rect_fusee.x = 100;
    rect_fusee.y = longueur - 200;

    SDL_Rect rect_hekate;	//Rect hekate
    rect_hekate.w = sigpatch_hekate_s->w;
    rect_hekate.h = sigpatch_hekate_s->h;
    rect_hekate.x = 960;
    rect_hekate.y = longueur - 200;

    SDL_Rect rect_updateApp;	//Rect hekate
    rect_updateApp.w = updateApp_s->w;
    rect_updateApp.h = updateApp_s->h;
    rect_updateApp.x = largeur / 2 - rect_updateApp.w / 2;
    rect_updateApp.y = longueur - 200;

    SDL_Rect rect_cursorApp;
    rect_cursorApp.w = sigpatch_fusee_s->w + 50;
    rect_cursorApp.h = sigpatch_fusee_s->h + 20;
    rect_cursorApp.x = largeur / 2 - rect_updateApp.w / 2 - 50;
    rect_cursorApp.y = longueur - 210;

    SDL_Rect rect_title;	//Rect title
    rect_title.w = title_s->w;
    rect_title.h = title_s->h;
    rect_title.x = largeur / 2 - rect_title.w / 2;
    rect_title.y = 20;

    SDL_Rect rect_credit;	//Rect title
    rect_credit.w = credit_s->w;
    rect_credit.h = credit_s->h;
    rect_credit.x = largeur / 2 - rect_credit.w / 2;
    rect_credit.y = longueur - 40;   

    SDL_Rect rect_description;	//Rect title
    rect_description.w = description_s->w;
    rect_description.h = description_s->h;
    rect_description.x = largeur / 2 - rect_description.w / 2;
    rect_description.y = 100;  

    SDL_Rect rect_download;	//Rect title
    rect_download.w = download_s->w;
    rect_download.h = download_s->h;
    rect_download.x = largeur / 2 - rect_download.w / 2;
    rect_download.y = longueur / 2 - rect_download.h / 2;

    SDL_Rect rect_downloadEnd;	//Rect title
    rect_downloadEnd.w = downloadEnd_s->w;
    rect_downloadEnd.h = downloadEnd_s->h;
    rect_downloadEnd.x = largeur / 2 - rect_downloadEnd.w / 2;
    rect_downloadEnd.y = longueur / 2 - rect_downloadEnd.h / 2;

    SDL_FreeSurface(sigpatch_fusee_s); //We Delete Surface
    SDL_FreeSurface(sigpatch_hekate_s);
    SDL_FreeSurface(title_s);
    SDL_FreeSurface(description_s);
    SDL_FreeSurface(credit_s);
    SDL_FreeSurface(download_s);
    SDL_FreeSurface(downloadEnd_s);

    bool isOpen = true;		//mainLoop
    int choice = 0;		//Choice for cursor
    int downloadIsFinish = 0;
    SDL_Event event;	//Event
    chdir("sdmc:/");    //Go in the sdmc

    while (isOpen && appletMainLoop())
    {

        SDL_SetRenderDrawColor(mainRenderer, 45, 45, 45, 255);  //Render Color (grey)

        if (choice >= 3)
        {
            choice = 0;
        }
        if (downloadIsFinish == 1 && choice >= 2)
        {
            choice = 0;
        }
        

        if (choice <= -1)
        {
            choice = 2;
        }
      
        SDL_RenderClear(mainRenderer);  //We clear the screen

        while(SDL_PollEvent(&event)){   //Events
            if (event.type == SDL_QUIT)
                isOpen = false;
            
            if (event.type == SDL_JOYBUTTONDOWN)    //Pressed key
            {
                if (event.jbutton.button == 14)	//right
                {
                    ++choice;
                }

                if (event.jbutton.button == 12)	//left
                {
                    --choice;
                }

                if (event.jbutton.button == 10) // +
                {
                    isOpen = false;
                    break;
                }

                if (event.jbutton.button == 0 && choice == 0 && downloadIsFinish == 0)   //Button A and cursor on fusee
                {
                    SDL_RenderCopy(mainRenderer, download, NULL, &rect_download);
                    SDL_RenderPresent(mainRenderer);
                    CurlRequests *sessionCurl = new CurlRequests;                    
                    sessionCurl->downloadFile("patch.zip", "https://github.com/THZoria/patches/releases/latest/download/fusee.zip", false);
                    
                    unzipRequests *sessionUnzip = new unzipRequests;
                    sessionUnzip->unzipPatches();
                    downloadIsFinish = 1;
                }

                if (event.jbutton.button == 0 && choice == 2 && downloadIsFinish == 0)   //Button A and cursor on hekate
                {
                    SDL_RenderCopy(mainRenderer, download, NULL, &rect_download);
                    SDL_RenderPresent(mainRenderer);
                    CurlRequests *sessionCurl = new CurlRequests;
                    sessionCurl->downloadFile("patch.zip", "https://github.com/THZoria/patches/releases/latest/download/hekate.zip", false);

                    unzipRequests *sessionUnzip = new unzipRequests;
                    sessionUnzip->unzipPatches();
                    downloadIsFinish = 1;
                }
                
                if (event.jbutton.button == 0 && choice == 1 && downloadIsFinish == 0)   //Button A and cursor on app
                {
                    SDL_RenderCopy(mainRenderer, download, NULL, &rect_download);
                    SDL_RenderPresent(mainRenderer);
                    CurlRequests *sessionCurl = new CurlRequests;
                    sessionCurl->downloadFile("sigpatch-downloader.nro", "https://github.com/PoloNX/sigpatch-downloader/releases/latest/download/sigpatch-downloader.nro", true);

                    downloadIsFinish = 1;
                }

                if (event.jbutton.button == 0 && choice == 0 && downloadIsFinish == 1)
                {
                    reboot_to_payload("sdmc:/atmosphere/reboot_payload.bin");
                }

                if (event.jbutton.button == 0 && choice == 1 && downloadIsFinish == 1)
                {
                    reboot_to_payload("sdmc:/hekate_ctcaer_5.5.6.bin");
                }
            }
        }


        if (downloadIsFinish == 0){
            //render of text
            SDL_RenderCopy(mainRenderer, sigpatch_fusee, NULL, &rect_fusee); //print de text (fusee)
            SDL_RenderCopy(mainRenderer, sigpatch_hekate, NULL, &rect_hekate); //print de text (hekate)
            SDL_RenderCopy(mainRenderer, title, NULL, &rect_title); //print de text (title)
            SDL_RenderCopy(mainRenderer, description, NULL, &rect_description);
            SDL_RenderCopy(mainRenderer, credit, NULL, &rect_credit);
            SDL_RenderCopy(mainRenderer, hekateLogo, NULL, &rect_hekateLogo);
            SDL_RenderCopy(mainRenderer, amsLogo, NULL, &rect_amsLogo);
            SDL_RenderCopy(mainRenderer, updateApp, NULL, &rect_updateApp);

        }
        else if (downloadIsFinish == 1){
            SDL_RenderCopy(mainRenderer, downloadEnd, NULL, &rect_downloadEnd);
            SDL_RenderCopy(mainRenderer, sigpatch_hekate, NULL, &rect_hekate);
            SDL_RenderCopy(mainRenderer, sigpatch_fusee, NULL, &rect_fusee);
        }

        if (choice == 0) //Draw cursor on fusee
        {
            SDL_SetRenderDrawColor(mainRenderer, 123, 224, 228, 255);
            SDL_RenderDrawRect(mainRenderer, &rect_cursorAms);
        }

        if (choice == 1) //Draw cursor on hekate
        {
            if (downloadIsFinish == 0)
            {
                SDL_SetRenderDrawColor(mainRenderer, 123, 224, 228, 255);
                SDL_RenderDrawRect(mainRenderer, &rect_cursorApp);
            }

            if (downloadIsFinish == 1)
            {
                SDL_SetRenderDrawColor(mainRenderer, 123, 224, 228, 255);
                SDL_RenderDrawRect(mainRenderer, &rect_cursorHekate);
            }
        }

        if (choice == 2 && downloadIsFinish == 0) //Draw cursor on app
        {
            SDL_SetRenderDrawColor(mainRenderer, 123, 224, 228, 255);
            SDL_RenderDrawRect(mainRenderer, &rect_cursorHekate);
        }

        SDL_RenderPresent(mainRenderer); //Render Present

    }

    SDL_Quit(); //Quit
    return 0;
}
