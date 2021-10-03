#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include "State.hpp"

#include "Packet.hpp"

class Packet;

typedef bool (*packet_parsing_func)(Packet*,State*);

class ProtocolFunction {
public:
    std::string name;
    packet_parsing_func function;
public:
    ProtocolFunction();
    ProtocolFunction(std::string,packet_parsing_func);
    ~ProtocolFunction();
    void operator()(Packet*,State*);
};

struct ProtocolMode {
public:
    std::string name;
    std::unordered_map<uint32_t,ProtocolFunction> functions;
public:
    ProtocolMode();
    ProtocolMode(std::string,std::unordered_map<uint32_t,ProtocolFunction>);
    ~ProtocolMode();
    bool functionDefined(uint32_t);
    ProtocolFunction* operator[](uint32_t);
};

class ProtocolInfo {
public:
    std::string name;
    std::unordered_map<uint32_t,ProtocolMode> modes;
public:
    ProtocolInfo();
    ProtocolInfo(std::string,std::unordered_map<uint32_t,ProtocolMode>);
    ~ProtocolInfo();
    bool modeDefined(uint32_t);
    ProtocolMode* operator[](uint32_t);
};
