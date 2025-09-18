#include "player.h"

player::player(const std::string& playerName, const std::string& playerUUID)
    : name(playerName), uuid(playerUUID) {}

std::string player::getName() const {
    return name;
}

std::string player::getUUID() const {
    return uuid;
}