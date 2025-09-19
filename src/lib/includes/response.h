#ifndef RESPONSE_H
#define RESPONSE_H

#include "tools.h"
#include "packet.h"
#include "player.h"
#include "gameEngine.h"

#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

// TODO: add parameters for max players and online players
void sendHandShake(int clientSocket);

void sendPong(int clientSocket, string data);

void sendLoginSuccess(int clientSocket, string uuid, string name);

void sendKnownPacks(int clientSocket);

void sendFinishConfiguration(int clientSocket);

void sendLoginPlay(int clientSocket, player* p);

#endif // RESPONSE_H