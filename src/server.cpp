// C++ program to show the example of server application in
// socket programming
#include <packet.h>
#include <tools.h>
#include <response.h>
#include <player.h>

#define PORT 25565
#define MAX_PLAYERS 5

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

using namespace std;

// client state management
int state = 0;

// player management
int numPlayers = 0;
player* players[MAX_PLAYERS];


bool interpretPacket(int clientSocket, Packet& packet) {
    switch (packet.getPacketId())
    {
    case 0x00: // Handshake
        if (state == 0) {
            state = packet.getContent()[packet.getContent().size() - 1];
        }
        else if (state == 1) {
            sendHandShake(clientSocket);
            state = 0;
        }
        else if (state == 2) {
            if (numPlayers < MAX_PLAYERS)
            {
                player* newPlayer = new player(packet.readString(), packet.readUUID());
                players[numPlayers++] = newPlayer;
                cout << "Player " << newPlayer->getName() << " with UUID " << parseUUID(newPlayer->getUUID()) << " is connecting." << endl;

                sendLoginSuccess(clientSocket, newPlayer->getUUID(), newPlayer->getName());
                cout << "Login success sent." << endl;
            }
            
            // login state
            state = 0;
        }
        cout << "State: " << state << endl;
        break;

    case 0x01: // Ping
        sendPong(clientSocket, packet.getContent());
        return false; // close connection after pong

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
        state = 0;
        cout << "Closing connection" << endl;
        close(clientSocket);
    }

    cout << "Server closed" << endl;

    return 0;
}

