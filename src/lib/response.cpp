#include "response.h"

void sendHandShake(int clientSocket) {
    stringstream ss;
    string handshakeData = "{\"version\":{\"name\":\"1.21.8\",\"protocol\":772},"
        "\"players\":{\"max\":5, \"online\":0},"
        "\"description\":{\"text\":\"My minecraft project\"}}";
    cout << "Handshake data: " << handshakeData << endl;
    ss << getHeaderBufferSize(handshakeData) << handshakeData;
    Packet handshakePacket(0x00, ss.str());
    handshakePacket.sendPacket(clientSocket);
}

void sendPong(int clientSocket, string data) {
    Packet pongPacket(0x01, data);
    pongPacket.sendPacket(clientSocket);
}