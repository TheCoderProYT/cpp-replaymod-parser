#pragma once

#include <cstdint>

class State {
private:
    uint32_t connectionMode = 2;
public:
    State();
    ~State();

    uint32_t getConnectionMode();
    void setConnectionMode(uint32_t);
};