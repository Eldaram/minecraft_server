// C++ program to show the example of server application in
// socket programming
#include <packet.h>
#include <tools.h>
#include <response.h>
#include <player.h>

#define PORT 25565

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

using namespace std;

// client state management
enum State {
    HANDSHAKE = 0,
    STATUS = 1,
    LOGIN = 2,
    PLAY = 3
};
int state = State::HANDSHAKE;

// player management
int numPlayers = 0;
player* players[MAX_PLAYERS];


bool interpretPacket(int clientSocket, Packet& packet) {
    switch (packet.getPacketId())
    {
    case 0x00: // Handshake
        if (state == State::HANDSHAKE) {
            state = packet.getContent()[packet.getContent().size() - 1];
        }
        else if (state == State::STATUS) {
            sendHandShake(clientSocket);
            state = State::HANDSHAKE;
        }
        else if (state == State::LOGIN) {
            if (numPlayers < MAX_PLAYERS)
            {
                player* newPlayer = new player(packet.readString(), packet.readUUID());
                players[numPlayers++] = newPlayer;
                cout << "Player " << newPlayer->getName() << " with UUID " << parseUUID(newPlayer->getUUID()) << " is connecting." << endl;

                sendLoginSuccess(clientSocket, newPlayer->getUUID(), newPlayer->getName());
                cout << "Login success sent." << endl;
            }
            
            // login state
            state = State::HANDSHAKE;
        }
        break;

    case 0x01: // Ping
        sendPong(clientSocket, packet.getContent());
        return false; // close connection after pong

    case 0x03: // (log) Login acknowledge OR (play) acknowledge finish config
        if (state == State::LOGIN)
            sendKnownPacks(clientSocket);
        else if (state == State::PLAY)
        {
            /* code */
        }
        break;

    case 0x07: //Serverbound known packs
        sendFinishConfiguration(clientSocket);
        state = State::PLAY;
        break;

    default:
        break;
    }

    return true;
}

int main()
{
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    cout << "Server started" << endl;

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    while (true) {
        // accepting connection request
        int clientSocket
            = accept(serverSocket, nullptr, nullptr);

        // recieving data

        bool connected = true;
        Packet* packet;
        while (connected) {
            packet = new Packet(clientSocket);
            connected = interpretPacket(clientSocket, *packet);
            free(packet);
        }
        
        // closing the socket.
        state = State::HANDSHAKE;
        cout << "Closing connection" << endl;
        close(clientSocket);
    }

    cout << "Server closed" << endl;

    return 0;
}

