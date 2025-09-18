#ifndef RESPONSE_H
#define RESPONSE_H

#include "tools.h"
#include "packet.h"

#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

void sendHandShake(int clientSocket);

void sendPong(int clientSocket, string data);

#endif // RESPONSE_H