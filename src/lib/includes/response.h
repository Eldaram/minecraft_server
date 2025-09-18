#ifndef RESPONSE_H
#define RESPONSE_H

#include "tools.h"
#include "packet.h"

#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

// TODO: add parameters for max players and online players
void sendHandShake(int clientSocket);

void sendPong(int clientSocket, string data);

void sendLoginSuccess(int clientSocket, string uuid, string name);

#endif // RESPONSE_H