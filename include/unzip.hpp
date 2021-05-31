#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <dirent.h>
#include <minizip/unzip.h> 
#include <switch.h>


class unzipRequests 
{
    public:

        unzipRequests();

        bool unzipPatches();

};
