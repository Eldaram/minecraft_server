// custom libraries
#include "tools.h"

// curlpp for HTTP requests (API calls)
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

// jsoncpp for JSON parsing
#include <json/json.h>

#include <iostream>
#include <memory>

bool testBasic();

bool testBasic()
{
  Json::Value playerData = getPlayerDetailsFromMojangAPI("49bf3ed4031f4d37b6ff0a51917d5e09");
  std::cout << "Player name from API: " << playerData["name"].asString() << std::endl;
  return playerData["name"].asString() == "Eldaram12";
}

int main(int, char**)
{
  if (!testBasic())
    return 1;

  return 0;
}