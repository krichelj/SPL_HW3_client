#ifndef BOOST_ECHO_CLIENT_SERVERTOCLIENT_H
#define BOOST_ECHO_CLIENT_SERVERTOCLIENT_H

#include <ConnectionHandler.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class ServerToClientDecoder{

private:
    ConnectionHandler* currentConnectionHandler;
    mutex* logoutMutex;
    condition_variable* conditionVariable;
    bool shouldTerminate;

public:
    ServerToClientDecoder(ConnectionHandler* currentConnectionHandler, mutex* logoutMutex, condition_variable* conditionVariable); // constructor
    void operator()(); // thread runner method
    short bytesToShort(char* bytesArr);
    void terminate(); // termination method

};



#endif //BOOST_ECHO_CLIENT_SERVERTOCLIENT_H
