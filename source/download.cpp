#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cctype>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <curl/curl.h>

#include <download.hpp>
#include <utils.hpp>

#define Megabytes_in_Bytes	1048576
#define Kibibyte_in_Bytes	1024
#define TIMETYPE double
#define TIMEOPT CURLINFO_TOTAL_TIME
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3
#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         6000

int dlnow_Mb = 0;
int dltotal_Mb = 0;

// measure download speed
bool open_room = false;
bool once = false;
int dlspeed = 0;
int dl_curr = 0;
int	curr_sec = 0; // current second from system
int ticket = 0; // current (second + 1) from system

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::string data((const char*) ptr, (size_t) size * nmemb);
    *((std::stringstream*) stream) << data;
    return size * nmemb;
}

size_t download_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

CurlRequests::CurlRequests(SDL_Window* window, int horizontal, int vertical, SDL_Renderer* renderer, TTF_Font* font)
{
    this->window = window;
    this->renderer = renderer;
    this->font = font;
    this->horizontal = horizontal;
    this->vertical = vertical;
}

int xferinfo(curl_off_t dltotal, curl_off_t dlnow) {

    dlnow_Mb = dlnow / Megabytes_in_Bytes;
    dltotal_Mb = dltotal / Megabytes_in_Bytes;

    // we need to create a separated room inside this "while" loop
    // so we can process this room once every so often
    curr_sec = time(0);

    if (open_room == false) {
        ticket = time(0) + 1;
        dl_curr = dlnow;
        open_room = true; // closing room
    }

    if (curr_sec >= ticket) {
        dlspeed = (dlnow - dl_curr) / Kibibyte_in_Bytes;
        open_room = false; // opening room
    }

    //utilsRequests utilsProgressBar(window, horizontal, vertical, renderer, font);
    //utilsProgressBar.printProgressBar(dlnow_Mb, dltotal_Mb, dlspeed);

    if (dltotal_Mb == 1) {
        printf("# DOWNLOAD: %" CURL_FORMAT_CURL_OFF_T " Bytes of %" CURL_FORMAT_CURL_OFF_T " Bytes | %3d Kb/s\r", dlnow, dltotal, dlspeed);
    } else if (dltotal_Mb > 1) {
        printf("# DOWNLOAD: %d Mb of %d Mb | %3d Kb/s\r", dlnow_Mb, dltotal_Mb, dlspeed);
    }

    if (dlnow == dltotal && dltotal > 0 && once == false) {
        printf("\n                                                                                "); // lol, is required
        once = true;
    }

    consoleUpdate(NULL);
    return 0;
}



int older_progress(__attribute__((unused)) void* p, double dltotal, double dlnow, __attribute__((unused)) double ultotal, __attribute__((unused)) double ulnow) {
    return xferinfo((curl_off_t)dltotal, (curl_off_t)dlnow);
}

struct myprogress {
  TIMETYPE lastruntime;
  CURL *curl;
};
char global_f_tmp[512]; /* we need this global FILE variable for passing args */

/* Functions */


bool folder_exists(std::string foldername)
    {
        struct stat st;
        stat(foldername.c_str(), &st);
        return st.st_mode & S_IFDIR;
    }


int _mkdir(const char *path)
    {

        return ::mkdir(path,0755); // not sure if this works on mac

    }

int mkdirMod(const char *path)
    {
        std::string current_level = "";
        std::string level;
        std::stringstream ss(path);

        // split path using slash as a separator
        while (std::getline(ss, level, '/'))
        {
            current_level += level; // append folder to the current level

            // create current level
            if (!folder_exists(current_level) && _mkdir(current_level.c_str()) != 0)
                return -1;

            current_level += "/"; // don't forget to append a slash
        }

        return 0;
    }

bool CurlRequests::downloadFile(const char *filename, const char *urlPatches, bool appUpdater)
{
    DIR* dir;
    struct dirent* ent;
    if (appUpdater == true){    
        chdir ("switch/");
        mkdirMod ("sigpatch-downloader");
    }
            while ((ent = readdir(dir)))
            {
                std::cout <<  ent->d_name << std::endl;
            }
            closedir(dir);

    if (appUpdater == true)
        chdir ("sigpatch-downloader/");
    
    FILE				*dest = NULL;
    CURL				*curl = NULL;
    CURLcode			res;
    struct myprogress	prog;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    curl = curl_easy_init();
    if (curl) {
        prog.lastruntime = 0;
        prog.curl = curl;

        dest = fopen(filename, "wb");
        if (dest == NULL) {
            perror("fopen");
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, urlPatches);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:84.0) Gecko/20100101 Firefox/84.0");// getting URL from char *url
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);// useful for debugging
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 			// skipping cert. verification, if needed
            //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 			// skipping hostname verification, if needed
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, dest);				// writes pointer into FILE *destination
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, older_progress);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

            if (strlen(global_f_tmp) != 0) curl_easy_setopt(curl, CURLOPT_USERPWD, global_f_tmp);

            res = curl_easy_perform(curl);	// perform tasks curl_easy_setopt asked before

            fclose(dest);
        }
    }

    curl_easy_cleanup(curl);												// Cleanup chunk data, resets functions.
    if (res != CURLE_OK) {
            if (appUpdater == false)
            {
                remove(filename);
            }
            std::cout << "erreur curl" << std::endl;
            return false;
        }
    
    rename ("temp.nro", "sigpatch-downloader.nro");

        return true;

}
