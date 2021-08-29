#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

typedef void (*packet_parsing_func)(void);

struct ProtocolFunction {
  std::string name;
  packet_parsing_func function;
  void operator()() {
    function();
  }
};

struct ProtocolMode {
  std::string name;
  std::unordered_map<uint32_t,ProtocolFunction> functions;
};

struct ProtocolInfo {
  std::string name;
  std::unordered_map<uint32_t,ProtocolMode> modes;
};
