#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

typedef void (*packet_parsing_func)(uint32_t,uint8_t*);

struct ProtocolInfo {
  uint32_t id;
  std::string name;
  std::unordered_map<uint64_t,packet_parsing_func> functions;
};
