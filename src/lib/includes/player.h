#ifndef PLAYER_H
#define PLAYER_H

#define MAX_PLAYERS 5

#include <string>

class player
{
private:
    std::string name;
    std::string uuid;
    int8_t dimension = 0; // value in the dimensionNames array (see gameEngine.h)
    int8_t gameMode; // default game mode

public:
    player(const std::string& playerName, const std::string& playerUUID);

    enum GameMode {
        UNDEFINED = -1,
        SURVIVAL = 0,
        CREATIVE = 1,
        ADVENTURE = 2,
        SPECTATOR = 3
    };

    std::string getName() const;
    std::string getUUID() const;
    int8_t getDimension() const;
    int8_t getGameMode() const;

    void setDimension(int8_t dim);
    void setGameMode(int8_t mode);
};

#endif // PLAYER_H