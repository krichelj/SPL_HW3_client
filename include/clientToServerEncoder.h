#ifndef BOOST_ECHO_CLIENT_CLIENTTOSERVER_H
#define BOOST_ECHO_CLIENT_CLIENTTOSERVER_H

#include <thread>
#include <connectionHandler.h>

class ClientToServerEncoder {

private:
    ConnectionHandler &currentConnectionHandler_;
    bool shouldTerminate_;

public:
    explicit ClientToServerEncoder(ConnectionHandler& currentConnectionHandler); // constructor
    virtual ~ClientToServerEncoder(); // destructor
    void operator()(); // thread runner method.
    void terminate(); // termination method

};


#endif //BOOST_ECHO_CLIENT_CLIENTTOSERVER_H
