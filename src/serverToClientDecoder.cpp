#include <serverToClientDecoder.h>
#include <boost/algorithm/string.hpp>

using namespace std;

// constructor
ServerToClientDecoder::ServerToClientDecoder(ConnectionHandler& currentConnectionHandler): currentConnectionHandler_(currentConnectionHandler),
shouldTerminate_(false) {}

// destructor
ServerToClientDecoder::~ServerToClientDecoder() {

}

void ServerToClientDecoder::operator()() {

    while (!shouldTerminate_){

        //From here we will see the rest of the echo client implementation:
        while (true) {

            const short bufsize = 1024;
            char buf[bufsize];
            cin.getline(buf, bufsize); // inputs commands from the user
            string line(buf); // creates a line from the commands
            int len= static_cast<int>(line.length());

            vector<string> strs;
            boost::split(strs,line,boost::is_any_of(" "));

            if (!currentConnectionHandler_.sendLine(line)) {
                cout << "Disconnected. Exiting...\n" << endl;
                break;
            }
            // connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
            cout << "Sent " << len+1 << " bytes to server" << endl;

            // We can use one of three options to read data from the server:
            // 1. Read a fixed number of characters
            // 2. Read a line (up to the newline character using the getline() buffered reader
            // 3. Read up to the null character
            string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
            if (!currentConnectionHandler_.getLine(answer)) {
                cout << "Disconnected. Exiting...\n" << endl;
                break;
            }

            len= static_cast<int>(answer.length());
            // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
            // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
            answer.resize(static_cast<unsigned long>(len - 1));
            cout << "Reply: " << answer << " " << len << " bytes " << endl << endl;
            if (answer == "bye") {
                cout << "Exiting...\n" << endl;
                break;
            }
        }
}

