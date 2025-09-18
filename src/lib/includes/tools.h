#ifndef TOOLS_H
#define TOOLS_H

#include <cstring>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// custom libraries
// curlpp for HTTP requests (API calls)
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

// jsoncpp for JSON parsing
#include <json/json.h>

// Function to receive all data from the client
ssize_t recvAll(int socket, char* buffer, size_t length);

// Function to read a VarInt from the socket
int readVarInt(int socket);

// create char buffer of size length
char getHeaderBufferSize(std::string data);

//parse UUID from string to the char decimal representation without dashes
std::string parseUUID(const std::string& uuid);

// Function to get the player detail from Mojang API with the given UUID
Json::Value getPlayerDetailsFromMojangAPI(const std::string& uuid);

#endif // TOOLS_H