#include "socketInit.hpp"

time_t    socketInit::_timeNow( void )
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec;
}

void   socketInit::_log( char* httpBuffer, int requistLen ) {

    int i = 0;
    std::string key, val;
    std::map<std::string, std::string> headerMap;
    while ( i <= requistLen ) {
    
        // parse ( method, url, http version ) first 
        if (i == 0) {

            while ( !(httpBuffer[i] == '\r' && httpBuffer[i + 1] == '\n') ) {
                val += httpBuffer[i++];
            }
            i += 2;
            headerMap["Method"] = val;
            val.clear();
        }
        
        // parse header element as key value
        while ( httpBuffer[i] != ':' )
            key += httpBuffer[i++];
        i += 2;
        
        // test for flag of valeu to ("\r\n") or first half of http_end ("\r\n\r\n")
        while ( !( httpBuffer[i] == '\r' && httpBuffer[i + 1] == '\n' ) ) {
            val += httpBuffer[i++];
        }
        i += 2;
        
        // add key, value to map
        headerMap[key] = val;
        
        // clear temporer buffer
        key.clear();
        val.clear();
        
        // test for flag of second half http_end ("\r\n\r\n")
        if ( httpBuffer[i] == '\r' && httpBuffer[i + 1] == '\n' )
                break;
    }
    
    time_t time = std::time(NULL);
    std::cout << "[" << std::put_time(std::gmtime(&time), "%c") 
              << "] [" << headerMap["Method"]
              << "] [" << headerMap["Host"]
              << "] [" << headerMap["User-Agent"] << "]"
              << &std::flush << std::endl;
}