#pragma once

#include <cstdint>
#include <fstream>
#include <unordered_map>
#include "CustomTypes.hpp"
#include <vector>

struct State {
    uint32_t connectionMode = 2;
    FILE* fileOutput;
    std::vector<User> players;
    
    State() {};
    ~State() {};
};