#include "socketInit.hpp"

socketInit::socketInit( void ) {

    int     fdPort;
    this->_tmpPort = 0;
    this->_clientCount = 0;


    FD_ZERO( &(this->_fdSet.rFds) );
    FD_ZERO( &(this->_fdSet.wFds) );

    for ( __vecServ::iterator it = gServers.begin(); it != gServers.end(); it++ ) {
        fdPort = this->_socketsUp( (*it).server_listen, (*it).server_host );

        this->_port.push_back((*it).server_listen);
        this->_fd.push_back(fdPort);
    }
    this->_clientCount = fdPort;
    this->_listen();
}

int    socketInit::_socketsUp( int port, std::string &host ) {

    std::stringstream   msErr;
    __S_IN              address, *res;
    int                 master_socket, optval = 1;
    std::string         portStr;


    std::memset(&address, 0, sizeof(address));
    address.ai_family = AF_INET;  
    address.ai_socktype = SOCK_STREAM;
    address.ai_protocol = IPPROTO_TCP;
    address.ai_canonname  = NULL;
    address.ai_addr  = NULL;
    address.ai_next  = NULL;

    portStr = std::to_string( port );
    if ( getaddrinfo( host.c_str(), portStr.c_str(), &address, &res ) )
        this->_err("interface error!");

    // create a master socket
    master_socket = socket( res->ai_family,
                            res->ai_socktype, 
                            res->ai_protocol );

    // socket optimisation for crash problem reuse ports
    setsockopt( master_socket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval) );  

    // bind the socket to localhost port
    msErr << "bind port[ " << port << " ]!"; 
    if ( bind( master_socket, res->ai_addr, res->ai_addrlen ) < 0 )
        this->_err(msErr.str().c_str());

    // free result of `getaddrinfo`
    freeaddrinfo (res);

    std::cout << "Listing On ( Host: "
              << host << ", Port: " << port << " )"
              << std::endl;

    if ( listen( master_socket, 1024 ) < 0 )
        this->_err("listen");
    return master_socket;
}

socketInit::~socketInit() {

    for ( std::vector<int>::iterator it = this->_fd.begin();
        it != this->_fd.end(); it++ ) 
        close( *it );
}

void    socketInit::_err( const char *statment ) {

    std::stringstream  err;
    err << statment << ": " <<  strerror(errno) << '\n';
    throw std::runtime_error(err.str().c_str());

}