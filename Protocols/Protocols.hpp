#pragma once
#include "../ProtocolInformation.hpp"
#include <unordered_map>

#include "Packets/756_2_2.hpp"
#include "Packets/756_3_21.hpp"

extern std::unordered_map<uint32_t,ProtocolInfo> protocols;