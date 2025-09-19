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

void sendLoginSuccess(int clientSocket, string uuid, string name) {
    Packet loginSuccessPacket(0x02, string());
    loginSuccessPacket.writeUUID(uuid);
    loginSuccessPacket.writeString(name);
    loginSuccessPacket.writeVarInt(0); // No properties
    //loginSuccessPacket.writeUserProperties(uuid);

    loginSuccessPacket.sendPacket(clientSocket);
}

void sendKnownPacks(int clientSocket) {
    Packet selectKnownPack(0x0e, string());

    //grid of 1 line and 3 columns
    string grid[3] = {"minecraft", "core", "1.21.8"};

    selectKnownPack.writePrefixedArray(grid, 1, 3);

    selectKnownPack.sendPacket(clientSocket);
}

void sendFinishConfiguration(int clientSocket) {
    Packet finishConfiguration(0x03, "");
    finishConfiguration.sendPacket(clientSocket);
}

void sendLoginPlay(int clientSocket, player* p) {
    Packet loginPlay(0x2b, string());

    loginPlay.writeVarInt(clientSocket); // Entity ID ==> TODO encode on 32 bits
    loginPlay.writeVarInt(0x00); // is hardcore
    loginPlay.writePrefixedArray(dimensionNames, dimensionNames->size(), 1); // Dimension names ==> TODO
    loginPlay.writeVarInt(MAX_PLAYERS); // Max players
    loginPlay.writeVarInt(VIEW_DISTANCE); // View distance
    loginPlay.writeVarInt(SIMULATION_DISTANCE); // Simulation distance
    loginPlay.writeVarInt(0x00); // Reduced debug info
    loginPlay.writeVarInt(0x01); // Enable respawn screen
    loginPlay.writeVarInt(0x00); // Do limited crafting
    loginPlay.writeVarInt(p->getDimension()); // Dimension type
    loginPlay.writeString(dimensionNames[p->getDimension()]); // Dimension name
    loginPlay.writeVarInt(0x00); // Hashed seed ==> TODO encode on 64 bits
    loginPlay.writeVarInt(p->getGameMode()); // Game mode
    loginPlay.writeVarInt(player::GameMode::UNDEFINED); // Previous gamemode ==> TODO
    loginPlay.writeVarInt(0x00); // is debug
    loginPlay.writeVarInt(0x01); // is flat world ==> TMP
    loginPlay.writeVarInt(0x00); // Has death location
    loginPlay.writeVarInt(PORTAL_COOLDOWN); // Portal cooldown
    loginPlay.writeVarInt(SEA_LEVEL); // sea level
    loginPlay.writeVarInt(0x00); // enforce secure chat
    

    loginPlay.sendPacket(clientSocket);

    cout << "Player " << p->getName() << " has entered the game." << endl;
}