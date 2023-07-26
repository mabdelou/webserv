#include "socketInit.hpp"

void   socketInit::_setFdSet( void ) {


    int     tmp[3];

    tmp[0] = 0;
    tmp[1] = 0;

    std::memset( tmp, 0, sizeof(tmp) );
    FD_ZERO( &(this->_fdSet.rFds) );
    FD_ZERO( &(this->_fdSet.wFds) );

    for ( std::vector<int>::iterator it = this->_fd.begin();
            it != this->_fd.end(); it++ )  {
        FD_SET( (*it), &(this->_fdSet.rFds) );
    }

    if ( gResponse.empty() && this->_client.empty() ) {

        gSockInfo.RfdSet.clear();
        gSockInfo.WfdSet.clear();
    }
    else {

        for ( std::vector<int>::iterator it = gSockInfo.RfdSet.begin();
                it != gSockInfo.RfdSet.end(); it++) {
            if ( *it > tmp[0] )
                tmp[0] = *it;
            if ( *it )
                FD_SET( (*it), &(this->_fdSet.rFds) );
        }
        for ( std::vector<int>::iterator it = gSockInfo.WfdSet.begin();
                it != gSockInfo.WfdSet.end(); it++) {
            if ( *it > tmp[1] )
                tmp[1] = *it;
            if ( *it )
                FD_SET( (*it), &(this->_fdSet.wFds) );
        }
    }
    tmp[2] = this->_fd.size() + 2;
    for ( int i = 0; i < 3; i++ ) {
        if ( this->_clientCount < tmp[i] )
            this->_clientCount = tmp[i];
    }
}

void   socketInit::_eraseFdSet( int fd, bool rw ) {

    std::vector<int>::iterator it;
    if ( rw == __WRITE_FD_SET__ ) {

        it = std::find( gSockInfo.WfdSet.begin(), gSockInfo.WfdSet.end(), fd );
        if (it != gSockInfo.WfdSet.end())
            gSockInfo.WfdSet.erase( it );
        return;
    }
    it = std::find( gSockInfo.RfdSet.begin(), gSockInfo.RfdSet.end(), fd );
    if (it != gSockInfo.RfdSet.end())
        gSockInfo.RfdSet.erase( it );
}

std::vector<tResponse>::iterator socketInit::_getHeader( int id ) {

    __lGr::iterator  itClientRes;
    for ( itClientRes =  gResponse.begin();
        itClientRes != gResponse.end(); itClientRes++ )
        if ( (*itClientRes).id ==  id )
            break;
    return ( itClientRes);
}

std::list<gFileInfo>::iterator socketInit::_findClient( int client ) {

    for ( std::list<gFileInfo>::iterator it = clientInfo.begin();
            it != clientInfo.end(); it++ )
        if ( (*it).id == client )
            return it;
    return std::end(clientInfo);
}

void    socketInit::_dropIt( std::vector<tResponse>::iterator itClientRes ) {

        std::vector<int>::iterator it;
        it = std::find( gSockInfo.RfdSet.begin(), gSockInfo.RfdSet.end(), itClientRes->id);
        if (it != gSockInfo.RfdSet.end())
            gSockInfo.RfdSet.erase( it );

        it = std::find( gSockInfo.WfdSet.begin(), gSockInfo.WfdSet.end(), itClientRes->id);
        if (it != gSockInfo.WfdSet.end())
            gSockInfo.WfdSet.erase( it );
        
        it = std::find( gSockInfo.RfdSet.begin(), gSockInfo.RfdSet.end(), itClientRes->fileFd);
        if (it != gSockInfo.RfdSet.end())
            gSockInfo.RfdSet.erase( it );

        it = std::find( gSockInfo.WfdSet.begin(), gSockInfo.WfdSet.end(), itClientRes->fileFd);
        if (it != gSockInfo.WfdSet.end())
            gSockInfo.WfdSet.erase( it );
}

