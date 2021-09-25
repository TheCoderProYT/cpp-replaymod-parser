#pragma once

#include "State.hpp"
#include <cstdint>
#include <iostream>

class Packet {
public:
    uint32_t length = 0;
    uint32_t timestamp = 0;
    uint8_t* data = nullptr;

    uint32_t ptr;

    uint32_t packetCount;
    size_t bufLocation;
public:
    Packet();
    ~Packet();
    
    bool run(State*);
};