#include <ServerToClientDecoder.h>
#include <boost/algorithm/string.hpp>

using namespace std;

// constructor
ServerToClientDecoder::ServerToClientDecoder(ConnectionHandler* currentConnectionHandler, mutex* logoutMutex, condition_variable* conditionVariable):
        currentConnectionHandler(currentConnectionHandler), logoutMutex(logoutMutex), conditionVariable(conditionVariable), shouldTerminate(false){

}

void ServerToClientDecoder::operator()() {

    while (!shouldTerminate) {

        char opCodeBytes[2];
        currentConnectionHandler->getBytes(opCodeBytes, 2); // get 2 bytes from the server for the opCode
        short opCode = bytesToShort(opCodeBytes); // convert the bytes to short

        if (opCode == 9){ // notification

            // get the notification type
            char typeBytes [1];
            currentConnectionHandler->getBytes(typeBytes, 1);

            string typeString, postingUser, content;

            if (typeBytes[0] == 48) // 0 char
                typeString = "PM";
            else
                typeString = "Public";

            // get the postingUser
            currentConnectionHandler->getFrameAscii(postingUser, '\0');
            postingUser = postingUser.substr(0, postingUser.size()-1);

            // get the content
            currentConnectionHandler->getFrameAscii(content, '\0');
            content = content.substr(0, content.size()-1);

            cout << "NOTIFICATION " << typeString << " " << postingUser << " "
                 << content << endl;

        }

        else if (opCode == 10){ // Ack

            char ackMessageOpCodeBytes[2];
            currentConnectionHandler->getBytes(ackMessageOpCodeBytes, 2); // get 2 bytes from the server for the message opCode of the ack
            short ackMessageOpCode = bytesToShort(ackMessageOpCodeBytes); // convert the bytes to short

            if (ackMessageOpCode == 3) {

                cout << "ACK " << ackMessageOpCode << endl; // print the desired output
                terminate(); // call to terminate the server to client thread
            }

            else if (ackMessageOpCode == 4 || ackMessageOpCode == 7){ // FollowUnfollowMessage or UserListMessage

                // record the number of users
                char numOfUsersBytes[2];
                currentConnectionHandler->getBytes(numOfUsersBytes, 2);
                short numOfUsers = bytesToShort(numOfUsersBytes);

                string userNamesString;

                for (int i = 0; i< numOfUsers ; i++){

                    string currentName;
                    currentConnectionHandler->getFrameAscii(currentName, '\0');
                    userNamesString += " " + currentName.substr(0, currentName.size()-1);
                }

                cout << "ACK " << ackMessageOpCode << " " << numOfUsers << userNamesString << endl;
            }

            else if (ackMessageOpCode == 8){ // Stat

                // get the numOfPosts
                char numBytes[2];
                currentConnectionHandler->getBytes(numBytes, 2);
                short numOfPosts = bytesToShort(numBytes);

                // get the numOfFollowers
                currentConnectionHandler->getBytes(numBytes, 2);
                short numOfFollowers = bytesToShort(numBytes);

                // get the numOfFollowing
                currentConnectionHandler->getBytes(numBytes, 2);
                short numOfFollowing = bytesToShort(numBytes);

                cout << "ACK " << ackMessageOpCode << " " << numOfPosts << " " << numOfFollowers
                     << " " << numOfFollowing << endl;

            }
            else
                cout << "ACK " << ackMessageOpCode << endl;

        }

        else if (opCode == 11){ // Error

            char errorMessageOpCodeBytes[2];
            currentConnectionHandler->getBytes(errorMessageOpCodeBytes, 2); // get 2 bytes from the server for the message opCode of the ack
            short errorMessageOpCode = bytesToShort(errorMessageOpCodeBytes); // convert the bytes to short

            cout << "ERROR " << errorMessageOpCode << endl;

            if (errorMessageOpCode == 3)
                conditionVariable->notify_all();// release the keyboard in case the logout was failed
        }
    }
}

short ServerToClientDecoder::bytesToShort(char *bytesArr) {

    auto result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

void ServerToClientDecoder::terminate() {

    shouldTerminate = true;
}
