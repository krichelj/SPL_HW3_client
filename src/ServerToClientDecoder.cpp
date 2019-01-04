#include <ServerToClientDecoder.h>
#include <boost/algorithm/string.hpp>

using namespace std;

// constructor
ServerToClientDecoder::ServerToClientDecoder(ConnectionHandler* currentConnectionHandler, mutex* logoutMutex, condition_variable* conditionVariable):
currentConnectionHandler(currentConnectionHandler), logoutMutex(logoutMutex), conditionVariable(conditionVariable), shouldTerminate(false){}

// destructor
ServerToClientDecoder::~ServerToClientDecoder() {

}

void ServerToClientDecoder::operator()() {

    while (!shouldTerminate) {

        char opCodeBytes[2];
        currentConnectionHandler->getBytes(opCodeBytes, 2); // get 2 bytes from the server for the opCode
        short opCode = bytesToShort(opCodeBytes); // convert the bytes to short

        if (opCode == 9){ // notification


        }

        else if (opCode == 10){ // Ack

            char ackMessageOpCodeBytes[2];
            currentConnectionHandler->getBytes(ackMessageOpCodeBytes, 2); // get 2 bytes from the server for the message opCode of the ack
            short ackMessageOpCode = bytesToShort(ackMessageOpCodeBytes); // convert the bytes to short


            if (ackMessageOpCode == 1){

                cout << "A user has registered successfully!" << endl;
            }
            else if (ackMessageOpCode == 2){

                cout << "A user has logged in successfully!" << endl;
            }
            else if (ackMessageOpCode == 3){ // LOGOUT

                cout << "The current user wants to log out! What a nerd!" << endl;
                terminate();
            }


        }

        else if (opCode == 11){ // Error

            char errorMessageOpCodeBytes[2];
            currentConnectionHandler->getBytes(errorMessageOpCodeBytes, 2); // get 2 bytes from the server for the message opCode of the ack
            short errorMessageOpCode = bytesToShort(errorMessageOpCodeBytes); // convert the bytes to short

            if (errorMessageOpCode == 1){

                cout << "The requested user to register is already registered!" << endl;
            }
            // in case the user to login is not registered or already logged in or a wrong password was typed in
            // or the current connection handler of the service already has a user logged in
            else if (errorMessageOpCode == 2){

                cout << "The requested user to login is not registered or already logged in or a wrong password was typed in" << endl;
                cout << "or the current connection handler of the service already has a user logged in!" << endl;
            }
            else if (errorMessageOpCode == 3){ // LOGOUT

                cout << "The user does not exist or already logged off!" << endl;
                conditionVariable->notify_all(); // release the keyboard in case the logout got an error
            }



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
