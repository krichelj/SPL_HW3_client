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

            shortToBytesAndSend(4);
            char followOrUnfollow[1];
            currentConnectionHandler->sendLine(commandFromUser[1]); // send the follow/unfollow
            currentConnectionHandler->sendLine(commandFromUser[2]); // send the NumOfUsers
            for (int i=1; i<=stoi(commandFromUser[2]); i++)
                currentConnectionHandler->sendLine(commandFromUser[i+2]); // send the NumOfUsers
        }






            /*if (!currentConnectionHandler.sendLine(inputLine)) {
                cout << "Disconnected. Exiting...\n" << endl;
                break;
            }
            // connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
            cout << "Sent " << length+1 << " bytes to server" << endl;

            // We can use one of three options to read data from the server:
            // 1. Read a fixed number of characters
            // 2. Read a line (up to the newline character using the getline() buffered reader
            // 3. Read up to the null character
            string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
            if (!currentConnectionHandler.getLine(answer)) {
                cout << "Disconnected. Exiting...\n" << endl;
                break;
            }

            length= static_cast<int>(answer.length());
            // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
            // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
            answer.resize(static_cast<unsigned long>(length - 1));
            cout << "Reply: " << answer << " " << length << " bytes " << endl << endl;
            if (answer == "bye") {
                cout << "Exiting...\n" << endl;
                break;
            }*/

    }
}

void ClientToServerEncoder::shortToBytesAndSend(short number) {

    char opCodeByteArray[2];
    opCodeByteArray[0] = static_cast<char>((number >> 8) & 0xFF);
    opCodeByteArray[1] = static_cast<char>(number & 0xFF);
    currentConnectionHandler->sendBytes(opCodeByteArray,2);
}

void ClientToServerEncoder::terminate() {

    shouldTerminate = true;
}


