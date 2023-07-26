#include "socketInit.hpp"

void socketInit::_listen( void ) {

    struct timeval timeOut;
    int res;

    timeOut.tv_sec = 0;
    timeOut.tv_usec = 300;

    if ( this->_fd.empty() )
        this->_err( "Error fatal no socket descriptor!");

    while ( 1337 ) {

        this->_setFdSet();
        res = select( this->_clientCount + 1, &(this->_fdSet.rFds),
                      &(this->_fdSet.wFds), NULL, &timeOut );      
        if ( res == -1 ) {
            gSockInfo.RfdSet.clear();
            gSockInfo.WfdSet.clear();
        }
        if ( ! ( gSockInfo.RfdSet.empty() && gSockInfo.WfdSet.empty() ) )
            this->_clientAction();
        this->_acceptConnections();
    }

}

void    socketInit::_clientAction( void ) {

    // new client
    this->_sendResive();
    // request
    if ( ! clientInfo.empty() ) {  
    // std::cout << "Request 1 "<< std::endl;
        request_parssing();
    // std::cout << "Request 2 "<< std::endl;
    }
    // // response
    if ( ! gRequest.empty() ) {

    // std::cout << "Response 1 "<< std::endl;
        Response::Responses();
    // std::cout << "Response 2: " << gResponse.size() << std::endl;
    }
}

void    socketInit::_acceptConnections( void ) {

    int     newConnection;
    int     value = 1;
    std::vector<uint16_t>::iterator pit;

    pit = this->_port.begin();

    // iterate port number for accept connections
    for ( std::vector<int>::iterator it = this->_fd.begin();
            it != this->_fd.end(); it++, pit++) {
        if ( FD_ISSET( *it, &(this->_fdSet.rFds) ) ) {

            this->_tmpPort = *pit;
            newConnection = accept( *it, NULL, NULL );
            if ( newConnection != -1 ) {
                // set the SO_NOSIGPIPE option on the socket file 
                //  descriptor as handling for `Broken pipe: 13`
                setsockopt( newConnection, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value) );
                this->_client.push_back( newConnection );
                gSockInfo.RfdSet.push_back( newConnection );
                gSockInfo.WfdSet.push_back( newConnection );
            }
        }
    }
}

void    socketInit::_sendResive( void ) {

    for ( std::vector<int>::iterator it = this->_client.begin();
            it != this->_client.end(); it++ ) {
        if ( (*it) )
            this->_clientRecive( (*it) );
    }

    for ( std::vector<int>::iterator it = std::find( this->_client.begin(),
            this->_client.end(), 0 ); it != this->_client.end(); 
            it = std::find( this->_client.begin(), this->_client.end(), 0 ))
        this->_client.erase( it );
    
    for ( std::vector<tResponse>::iterator it = gResponse.begin();
            it != gResponse.end(); it++ ) {
         if ( (*it).id )
            this->_clientSend( (*it).id );
    }

    for ( std::vector<tResponse>::iterator it = this->_getHeader( 0 );
            it != gResponse.end(); it = this->_getHeader( 0 ))
        gResponse.erase( it );
}