#include <ClientToServerEncoder.h>
#include <boost/algorithm/string.hpp>

using namespace std;

// constructor
ClientToServerEncoder::ClientToServerEncoder(ConnectionHandler* currentConnectionHandler, mutex* logoutMutex, condition_variable* conditionVariable):
currentConnectionHandler(currentConnectionHandler), logoutMutex(logoutMutex), conditionVariable(conditionVariable), shouldTerminate(false){}

// destructor
ClientToServerEncoder::~ClientToServerEncoder() {

}

// thread runner method
void ClientToServerEncoder::operator()() {

    while (!shouldTerminate){

        //From here we will see the rest of the echo client implementation:
        const short inputBufferSize = 1024;
        char buffer[inputBufferSize];
        cin.getline(buffer, inputBufferSize); // inputs commands from the user
        string inputLine(buffer); // creates a line from the commands
        int length = static_cast<int>(inputLine.length());

        vector<string> commandFromUser;
        boost::split(commandFromUser,inputLine,boost::is_any_of(" "));
        string command = commandFromUser[0];

        if (command == "REGISTER"){

            shortToBytesAndSend(1); // change the array of shorts to an array of bytes and send the opcode
            currentConnectionHandler->sendLine(commandFromUser[1]); // send the username
            currentConnectionHandler->sendLine(commandFromUser[2]); // send the password
        }

        else if (command == "LOGIN"){

            shortToBytesAndSend(2);
            currentConnectionHandler->sendLine(commandFromUser[1]); // send the username
            currentConnectionHandler->sendLine(commandFromUser[2]); // send the password
        }

        else if (command == "LOGOUT"){

            shortToBytesAndSend(3);
            unique_lock<mutex> logoutLock(*logoutMutex);
            conditionVariable->wait(logoutLock);
        }

        else if (command == "FOLLOW"){

            if (commandFromUser.size() > 2){ // check that there are at least 3 arguments

                shortToBytesAndSend(4); // send the opCode
                shortToBytesAndSend(static_cast<short>(stoi(commandFromUser[1]))); // send the followOrUnfollow

                auto numOfUsers = (short) stoi(commandFromUser[2]);
                shortToBytesAndSend(numOfUsers); // send the numOfUsers

                // send all the user names
                for (unsigned int i = 0; i < numOfUsers; i++)
                    currentConnectionHandler->sendLine(commandFromUser[i + 3]); // send the NumOfUsers
            }
        }
    }
}

void ClientToServerEncoder::shortToBytesAndSend(short number) {

    char ByteArray[2];
    ByteArray[0] = static_cast<char>((number >> 8) & 0xFF);
    ByteArray[1] = static_cast<char>(number & 0xFF);
    currentConnectionHandler->sendBytes(ByteArray,2);
}

void ClientToServerEncoder::terminate() {

    shouldTerminate = true;
}


