#pragma once

#include "../shared/shared.hpp"

typedef struct s_fdInfo {

    fd_set      wFds, rFds;     // fd_set

}   fdInfo;

class   socketInit :
    public  Response
{

    private:
        typedef std::vector<s_server>       __vecServ;
        typedef std::vector<s_location>     __vecloc;
        typedef std::vector<std::string>    __vecStr;
        typedef struct addrinfo             __S_IN;
        typedef std::map<int, int>          __iiMap;
        typedef std::map<int, __S_IN>       __adMap;
        typedef std::map<int, __S_IN>       __isMap;
        typedef std::list<gFileInfo>        __GfI;
        typedef std::vector<tResponse>      __lGr;
        typedef std::vector<int>            __IntList;

    private:
        __adMap                             _servaddrFd;    // port address
        fdInfo                              _fdSet;         // port fd fd_set
        int                                 _clientCount;   // number of clients
        __IntList                           _client;        // accepted clients
        std::vector<__isMap>                _files;         // files rw
        int                                 _tmpPort;
        std::vector<uint16_t>               _port;           // port fd
        std::vector<int>                    _fd;             // port fd

    private:
        int                                 _socketsUp( int, std::string& );
        void                                _listen( void );
        void                                _err( const char* );
        void                                _acceptConnections( void );
        void                                _clientAction( void );
        void                                _clientRecive( int );
        void                                _clientSend( int );
        void                                _readFile( void );
        void                                _writeFile( void );
        void                                _eraseFdSet( int fd, bool rw );
        void                                _setFdSet( void );
        __GfI::iterator                     _findClient( int );
        __vecServ::iterator                 _getServerFromPort( void );
        __lGr::iterator                     _getHeader( int );
        void                                _sendFile( __lGr::iterator );
        void                                _cgiHandler( __lGr::iterator );
        void                                _sendNextBuffer( __lGr::iterator );
        void                                _sendNextBufferCgi( __lGr::iterator );
        time_t                              _timeNow( void );
        void                                _log( char*, int );
        void                                _tryToErase( int );
        void                                _sendResive( void );
        void                                _dropIt( std::vector<tResponse>::iterator );


    public:
        socketInit();
        ~socketInit();

};
