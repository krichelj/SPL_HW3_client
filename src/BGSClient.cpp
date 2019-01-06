#include <stdlib.h>
#include <ConnectionHandler.h>
#include <ClientToServerEncoder.h>
#include <ServerToClientDecoder.h>

using namespace std;

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " host port" << endl << endl;
        return -1;
    }

    string host = argv[1]; // CLA host
    auto port = static_cast<short>(atoi(argv[2])); // CLA port

    ConnectionHandler connectionHandler(host, port); // create a new connectionHandler

    if (!connectionHandler.connect()) {
        cerr << "Cannot connect to " << host << ":" << port << endl;
        return 1;
    }

    mutex logoutMutex; // create a mutex for both running threads
    condition_variable conditionVariable; // create a condition variable to work with the mutex

    // create the decoders and encoders
    ClientToServerEncoder clientToServerEncoder(&connectionHandler, &logoutMutex, &conditionVariable);
    ServerToClientDecoder serverToClientDecoder(&connectionHandler, &logoutMutex, &conditionVariable);

    // run the encoder and decoder threads
    thread clientToServerEncoderThread(ref(clientToServerEncoder));
    thread serverToClientDecoderThread(ref(serverToClientDecoder));

    serverToClientDecoderThread.join(); // wait for the ack from logout to close the server to client thread
    clientToServerEncoder.terminate(); // notify the client to server that it has to terminate also
    conditionVariable.notify_all(); // release the condition variable to allow the client to server thread to close
    clientToServerEncoderThread.join(); // wait until the client to server thread closes
    connectionHandler.close(); // close the connection properly

    return 0;
}