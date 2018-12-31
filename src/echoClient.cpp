#include <stdlib.h>
#include <connectionHandler.h>
#include <clientToServerEncoder.h>
#include <serverToClientDecoder.h>
#include <boost/algorithm/string.hpp>

using namespace std;

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " host port" << endl << endl;
        return -1;
    }

    string host = argv[1];
    short port = static_cast<short>(atoi(argv[2]));
    
    ConnectionHandler connectionHandler(host, port); // create a new connectionHandler

    // create the decoders and encoders
    ClientToServerEncoder clientToServerEncoder (connectionHandler);
    ServerToClientDecoder serverToClientDecoder(connectionHandler);

    // run the encoder and decoder threads
    thread encoderThread(ref(clientToServerEncoder));
    thread decoderThread(ref(serverToClientDecoder));


    if (!connectionHandler.connect()) {
        cerr << "Cannot connect to " << host << ":" << port << endl;
        return 1;
    }

    return 0;
}
