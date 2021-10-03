#pragma once

#include <cstdint>
#include "Packet.hpp"
#include <fstream>

class PacketStream {
private:
    bool fileLoaded = false;
    std::ifstream inputFile;
    
    uint32_t packetIDs = 0;

    uint32_t protocolNumber;
public:
    size_t filesize = 0;
    char* filesizeString;

    size_t currentLocation = 0;
public:
    PacketStream();
    ~PacketStream();

    bool loadFile(std::string);
    void setProtocolNumber(uint32_t);

    Packet* next();
    bool hasMorePackets();
};