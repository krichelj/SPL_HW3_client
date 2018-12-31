#ifndef BOOST_ECHO_CLIENT_SERVERTOCLIENT_H
#define BOOST_ECHO_CLIENT_SERVERTOCLIENT_H

#include <connectionHandler.h>
#include <thread>

class ServerToClientDecoder{

private:
    ConnectionHandler& currentConnectionHandler_;
    bool shouldTerminate_;

public:
    ServerToClientDecoder(ConnectionHandler& currentConnectionHandler_); // constructor
    virtual ~ServerToClientDecoder(); // destructor
    void operator()(); // thread runner method
    void terminate(); // termination method

};



#endif //BOOST_ECHO_CLIENT_SERVERTOCLIENT_H
