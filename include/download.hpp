#include <stdio.h>
#include <stdlib.h>
#include <string>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include <curl/curl.h> 
 //
using namespace std ;
class CurlRequests 
{
    public:

        CurlRequests();
        
        //string returnDebug();
        bool downloadFile(const char *filename);
        int xferinfo(curl_off_t dltotal, curl_off_t dlnow);

};
