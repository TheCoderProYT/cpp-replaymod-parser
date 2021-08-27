#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

typedef void (*packet_parsing_func)(void);

struct ProtocolInfo {
  uint32_t id;
  std::string name;
  std::unordered_map<uint64_t,packet_parsing_func> functions;
};
