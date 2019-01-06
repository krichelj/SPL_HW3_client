#include <ClientToServerEncoder.h>

using namespace std;

// constructor

ClientToServerEncoder::ClientToServerEncoder(ConnectionHandler* currentConnectionHandler, mutex* logoutMutex, condition_variable* conditionVariable):
        currentConnectionHandler(currentConnectionHandler), logoutMutex(logoutMutex), conditionVariable(conditionVariable), shouldTerminate(false){

}

// thread runner method

void ClientToServerEncoder::operator()() {

    while (!shouldTerminate){

        const short inputBufferSize = 1024;
        char buffer[inputBufferSize];
        vector<string> commandFromUser; // a vector of strings for the keyboard input from the user

        cin.getline (buffer, inputBufferSize); // inputs commands from the user
        string inputLine (buffer); // create a line from the commands
        istringstream stringStream (inputLine); // create a string stream from the input line

        // break down the string stream into discrete strings
        for(string currentString ; stringStream >> currentString ;)
            commandFromUser.push_back(currentString);

        string commandName = commandFromUser[0]; // record the commandName name

        if (commandName == "REGISTER"){

            shortToBytesAndSend(1); // change the array of shorts to an array of bytes and send the opcode
            currentConnectionHandler->sendLine(commandFromUser[1]); // send the username
            currentConnectionHandler->sendLine(commandFromUser[2]); // send the password
        }

        else if (commandName == "LOGIN"){

            shortToBytesAndSend(2);
            currentConnectionHandler->sendLine(commandFromUser[1]); // send the username
            currentConnectionHandler->sendLine(commandFromUser[2]); // send the password
        }

        else if (commandName == "LOGOUT"){

            shortToBytesAndSend(3);
            unique_lock<mutex> logoutLock(*logoutMutex);
            conditionVariable->wait(logoutLock);
        }

        else if (commandName == "FOLLOW"){

            if (commandFromUser.size() > 2){ // check that there are at least 3 arguments

                shortToBytesAndSend(4);
                shortToBytesAndSend(static_cast<short>(stoi(commandFromUser[1]))); // send the followOrUnfollow

                auto numOfUsers = (short) stoi(commandFromUser[2]);
                shortToBytesAndSend(numOfUsers); // send the numOfUsers

                // send all the user names
                for (unsigned int i = 0; i < (unsigned int) numOfUsers; i++)
                    currentConnectionHandler->sendLine(commandFromUser[i + 3]); // send the NumOfUsers
            }
        }

        else if (commandName == "POST"){

            string postContent;

            if(commandFromUser.size() > 1)  // check there's content in the post
                postContent = commandFromUser[1];

            // add all of the post
            for (unsigned int i = 2; i < commandFromUser.size(); i++)
                postContent += " " + commandFromUser[i];

            shortToBytesAndSend(5);
            currentConnectionHandler->sendLine(postContent); // send the post content
        }

        else if (commandName == "PM"){

            if (commandFromUser.size() > 1) { //checks if message is not empty

                string pmContent;

                if (commandFromUser.size() > 2)
                    pmContent = commandFromUser[2];

                for (unsigned int i = 3; i < commandFromUser.size(); i++) {
                    pmContent += " " + commandFromUser[i];
                }
                shortToBytesAndSend(6);
                currentConnectionHandler->sendLine(commandFromUser[1]);
                currentConnectionHandler->sendLine(pmContent);
            }
        }

        else if (commandName == "USERLIST")
            shortToBytesAndSend(7); // send the op code


        else if (commandName == "STAT"){

            if (commandFromUser.size() == 2){

                shortToBytesAndSend(8); // send the op code
                currentConnectionHandler->sendLine(commandFromUser[1]);
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


