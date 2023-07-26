#include "socketInit.hpp"

void    socketInit::_clientSend( int client ) {

    __lGr::iterator itClientRes;
    itClientRes = this->_getHeader( client );

    if ( (*itClientRes).id != client || client == 0 )
        return;

    if ( FD_ISSET( client, &(this->_fdSet.wFds) ) ) {

        if ( (*itClientRes).status == TEXT__ ) {

            this->_sendFile( itClientRes );
            return;
        }
        if ( (*itClientRes).status == NEXT__ ) {

            this->_sendNextBuffer( itClientRes );
            return;
        }
        if ( (*itClientRes).status == CGI__ ) {

            this->_cgiHandler(itClientRes);
            return;
        }
        }
        if ( (*itClientRes).status == CGI_NEXT__ ) {

            this->_sendNextBufferCgi(itClientRes);
            return;
        }
        if ( (*itClientRes).status == ERR__ 
            || (*itClientRes).status == NO_BODY__) {

            send( client, (*itClientRes).headers.c_str(), (*itClientRes).headers.size(), 0 );
            if ( (*itClientRes).status == NO_BODY__ ) {
                (*itClientRes).status = END__;
                return;
            }
            send( client, (*itClientRes).str_err.c_str(), (*itClientRes).str_err.size(), 0 );
            this->_eraseFdSet( client, __WRITE_FD_SET__ );
            close( itClientRes->id );
            itClientRes->id = 0;
            return;
    }
    if ( (*itClientRes).status == END__ ) {

        this->_dropIt( itClientRes );
        if ( itClientRes->fileFd > 0 )
            close( itClientRes->fileFd );
        close( itClientRes->id );
        itClientRes->id = 0;
    }
}

void    socketInit::_clientRecive( int client ) {

    std::vector<int>::iterator          itId;
    std::list<gFileInfo>::iterator      it;
    std::vector<int>::iterator          it2;

    it = this->_findClient( client );

    if (  FD_ISSET( client, &(this->_fdSet.rFds) ) ) {
        if ( it == clientInfo.end() ) {

            gFileInfo newInfo;
            newInfo.collected = read ( client, newInfo.buff, MAXBUFF );
            if ( newInfo.collected <= 0 ) {

                this->_tryToErase( client );
                it2 = std::find ( gSockInfo.RfdSet.begin(), gSockInfo.RfdSet.end(), client);
                if ( it2 != gSockInfo.RfdSet.end() )
                    gSockInfo.RfdSet.erase( it2 );
                close ( client );
                return;
            }
            newInfo.status = __START__;
            newInfo.id = client;
            newInfo.time = this->_timeNow();
            if ( __RELEASE__ )
                this->_log( newInfo.buff, newInfo.collected );
            newInfo.serverPort = this->_tmpPort;
            clientInfo.push_back( newInfo );
            return;
        }
        if ( (*it).status == __START__  )
            (*it).status = __READ__ ;
        if ( (*it).status == __READ__ ) {
            (*it).collected = read ( client, (*it).buff, MAXBUFF );
            return;
        }
    }
    if ( (*it).status == __END__ )  {

        itId =  std::find( this->_client.begin(), this->_client.end(), client );
        if ( itId != this->_client.end() )
            *itId = 0;
        if ( it != clientInfo.end() )
            clientInfo.erase( it );
        this->_eraseFdSet( client, __READ_FD_SET__ );
    }
}
