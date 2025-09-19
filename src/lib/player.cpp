#include "player.h"

player::player(const std::string& playerName, const std::string& playerUUID)
    : name(playerName), uuid(playerUUID) {
    gameMode = GameMode::SURVIVAL; // default game mode
    }

std::string player::getName() const {
    return name;
}

std::string player::getUUID() const {
    return uuid;
}

int8_t player::getDimension() const {
    return dimension;
}

int8_t player::getGameMode() const {
    return gameMode;
}

void player::setDimension(int8_t dim) {
    dimension = dim;
}

void player::setGameMode(int8_t mode) {
    gameMode = mode;
}