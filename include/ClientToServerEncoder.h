#ifndef BOOST_ECHO_CLIENT_CLIENTTOSERVERENCODER_H
#define BOOST_ECHO_CLIENT_CLIENTTOSERVERENCODER_H

#include <thread>
#include <ConnectionHandler.h>
#include <mutex>
#include <condition_variable>

using namespace std;

class ClientToServerEncoder {

private:
    ConnectionHandler* currentConnectionHandler;
    mutex* logoutMutex;
    condition_variable* conditionVariable;
    bool shouldTerminate;

public:
    ClientToServerEncoder(ConnectionHandler* currentConnectionHandler, mutex* logoutMutex, condition_variable* conditionVariable); // constructor
    void operator()(); // thread runner method.
    void shortToBytesAndSend(short number);
    void terminate(); // termination method
};


#endif //BOOST_ECHO_CLIENT_CLIENTTOSERVERENCODER_H
