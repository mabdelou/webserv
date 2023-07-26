#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>
#include <sstream>
#include <map>
#include <vector>
#include <exception>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <netdb.h>
#include <string>
#include <signal.h>
#include <time.h>

#include "../parssing/request_parssing/parssing_files/Request.hpp"

#define MAXBUFF             5000
    
#define __START__           0
#define __READ__            1
#define __END__             2

#define __READ_FD_SET__     1
#define __WRITE_FD_SET__    0

#define ERR__               0
#define TEXT__              1
#define CGI__               2
#define NO_BODY__           3
#define NEXT__              4
#define END__               5
#define CGI_NEXT__          6

//  
#define ENTITY_TOO_LONG     0
#define METHOD_UNK          1
#define BAD_REQ             2
#define NOT_IMPLEMNT        3
#define ENTITY_TO_LARG      4


#define __RELEASE__         0


// set of file descriptor to select managment ------------
typedef struct tSockInfo                                //|
{                                                       //|
                                                        //|
    std::vector<int>        ports;                      //|
    std::vector<int>        WfdSet;                     //|
    std::vector<int>        RfdSet;                     //|
                                                        //|
}   sSockInfo;                                          //|
//--------------------------------------------------------
// config file parsing -----------------------------------
typedef struct t_location                               //|
{                                                       //|
    std::string                 client_location;        //|
    std::string                 location_root;          //|
    std::vector<std::string>    location_index;         //|
    std::vector<std::string>    location_root_index;    //|
    std::vector<std::string>    accepted_methods;       //|
    bool                        autoindex;              //|
    std::string                 return_url;             //|
    std::string                 upload_path;            //|
    std::string                 cgi_path;               //|
    std::map<std::string,std::string>    cgi_accept;    //|
                                                        //|
}     s_location;                                       //|
                                                        //|
typedef struct t_error_page                             //|
{                                                       //|
    std::vector<std::string> error_numbers;             //|
    std::string              error_file_path;           //|
                                                        //|
}     s_error_page;                                     //|
                                                        //|
// server --------------------------------------------- //|
typedef struct t_server                                 //|
{                                                       //|
                                                        //|
                                                        //|
    uint16_t                    server_listen;          //|
    std::string                 server_host;            //|
    uint32_t                    server_cb_size;         //|
    std::string                 root;                   //|
    std::vector<std::string>    index;                  //|
    std::vector<s_location>     locations;              //|
    std::vector<s_error_page>   error_page;             //|
    std::map<std::string, std::string> map_error_pages; //|
}     s_server;                                         //|
//--------------------------------------------------------

// struct shared socket to request parsing ---------------
typedef struct sFileInfo {                              //|
                                                        //|
   int      id;                                         //|
   int      status;                                     //|
   int      collected;                                  //|
   char     buff[ MAXBUFF ];                            //|
   int      serverPort;                                 //|
   time_t   time;                                       //|
                                                        //|
}           gFileInfo;                                  //|
//--------------------------------------------------------

// struct shared request to response maker ---------------
typedef struct sRequest                                 //|
{                                                       //|
    int id;                                             //|
    t_request_head  request;                            //|
    int             serverPort;                         //|
                                                        //|
}   tRequest;                                           //|
//--------------------------------------------------------

// struct shared response to socket send -----------------
typedef struct sResponse                                //|
{                                                       //|
    int             id;                                 //|
    int             pid;                                //|
    int             status;                             //|
    int             fileFd;                             //|
    time_t          inTime;                             //|
    std::string     headers;                            //|
    std::string     str_err;                            //|
    std::string     path;                               //|
    bool            cgi;                                //|
                                                        //|
}   tResponse;                                          //|
//--------------------------------------------------------

#include "../Response/Response.hpp"

extern std::vector<tRequest>    gRequest;
extern std::vector<tResponse>   gResponse;
extern std::list<gFileInfo>   clientInfo;
extern std::vector<s_server>    gServers;
extern sSockInfo                gSockInfo;
template <typename  T> 
typename T::iterator   getServerPort( T &servers, int Port ) {

    for ( typename T::iterator it = servers.begin(); it != servers.end(); it++ ) {
            if ( (*it).server_listen == Port)
                return ( it );
    }
    return ( servers.end() );
}