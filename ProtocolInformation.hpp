#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

typedef void (*packet_parsing_func)(void);

class ProtocolFunction {
private:
    packet_parsing_func function;
public:
    std::string name;
public:
    ProtocolFunction();
    ~ProtocolFunction();
    void operator()();
};

struct ProtocolMode {
private:
    std::unordered_map<uint32_t,ProtocolFunction> functions;
public:
    std::string name;
public:
    ProtocolMode();
    ~ProtocolMode();
    bool functionDefined(uint32_t);
    ProtocolFunction* operator[](uint32_t);
};

class ProtocolInfo {
private:
    std::unordered_map<uint32_t,ProtocolMode> modes;
public:
    std::string name;
public:
    ProtocolInfo();
    ~ProtocolInfo();
    bool modeDefined(uint32_t);
    ProtocolMode* operator[](uint32_t);
};
