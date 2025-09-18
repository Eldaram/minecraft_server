#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class player
{
private:
    std::string name;
    std::string uuid;

public:
    player(const std::string& playerName, const std::string& playerUUID);

    std::string getName() const;
    std::string getUUID() const;
};

#endif // PLAYER_H