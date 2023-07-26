#include "socketInit.hpp"


void   socketInit::_cgiHandler( __lGr::iterator res  ) {

    if ( send ( (*res).id, (*res).headers.c_str(), (*res).headers.size(), 0 ) == -1) {
            res->status = END__;
            return;
    }
    gSockInfo.RfdSet.push_back( res->fileFd );
    this->_eraseFdSet( res->id, __READ_FD_SET__ );
    if ( std::find ( gSockInfo.WfdSet.begin(), gSockInfo.WfdSet.end(), res->id) 
            == gSockInfo.WfdSet.end() ) {
        gSockInfo.WfdSet.push_back( res->id );
    }
    res->inTime = this->_timeNow(); 
    res->status = CGI_NEXT__;
    return;
}

void    socketInit::_sendNextBufferCgi( __lGr::iterator res ) {

    char            buffer[ MAXBUFF ];
    int             contentLength = 0;
    int             status = -100;

    waitpid( res->pid, &status, WNOHANG );
    if ( status == 0 ) {
        res->status = END__;
        return;
    }
    if ( res->inTime <= ( this->_timeNow() - 5 ) ) {
        kill(res->pid, SIGKILL);
        res->status = END__;
        return ;
    }
    if ( FD_ISSET( (*res).fileFd, &(this->_fdSet.rFds) ) ) {

        contentLength = read ( (*res).fileFd, buffer, MAXBUFF );
        if ( contentLength == -1  ) {
            res->status = END__;
            return ;
        }

    }
    if ( FD_ISSET( (*res).id, &(this->_fdSet.wFds) ) ) {

        if ( ( send ( res->id, buffer, contentLength, 0 )) == -1) {
            res->status = END__;
            return;
        }

    }
}

void    socketInit::_sendFile( __lGr::iterator res ) {

    char            buffer[ MAXBUFF ];
    int             contentLength, fd;

    fd = open ( (*res).path.c_str(), O_RDONLY );
    res->fileFd = fd;
    if ( fd == -1 )
        return;

    if ( send ( (*res).id, (*res).headers.c_str(), (*res).headers.size(), 0 ) == -1) {
            res->status = END__;
            return;
    }
    contentLength = read ( fd, buffer, MAXBUFF );
    if ( send ( (*res).id, buffer, contentLength, 0 ) == -1 ) {
        res->status = END__;
        return;
    }
    if ( contentLength < MAXBUFF ) {
        res->status = END__;
        return;
    }
    this->_eraseFdSet( res->id, __READ_FD_SET__ );
    gSockInfo.RfdSet.push_back( fd );
    if ( std::find ( gSockInfo.WfdSet.begin(), gSockInfo.WfdSet.end(), res->id) 
            == gSockInfo.WfdSet.end() ) {
        gSockInfo.WfdSet.push_back( res->id );
    }
    res->status = NEXT__;
    return;
}

void    socketInit::_sendNextBuffer( __lGr::iterator res ) {

    char            buffer[ MAXBUFF ];
    int             contentLength;

    if ( FD_ISSET( (*res).fileFd, &(this->_fdSet.rFds) ) ) {

        contentLength = read ( (*res).fileFd, buffer, MAXBUFF );
        if ( contentLength <= 0 ) {
            res->status = END__;
            return;
        }
    }
    if ( FD_ISSET( (*res).id, &(this->_fdSet.wFds) ) ) {

        if ( (send ( res->id, buffer, contentLength, 0)) == -1) {
            res->status = END__;
            return;
        }
    }
}

void    socketInit::_tryToErase( int element ) {

    std::vector<int>::iterator it;

    it = std::find( this->_client.begin(), this->_client.end(), element );
    if ( it != this->_client.end() )
        *it = 0;
}
