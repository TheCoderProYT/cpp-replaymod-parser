#pragma once

#include <unordered_map>
#include <vector>
#include "playerInfo.hpp"

struct GameState {
  std::vector<PlayerInfoObject> tabPlayers;
};
