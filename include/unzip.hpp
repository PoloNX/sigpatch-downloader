#include <minizip/unzip.h>
#include <switch.h>

#include <dirent.h>
#include <string.h>
#include <iostream>

using namespace std;
class unzipRequests
{
    public:

        unzipRequests();

        bool unzipPatches();
        bool unzipApp();

};
