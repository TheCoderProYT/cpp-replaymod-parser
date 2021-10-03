#pragma once

#include "State.hpp"
#include <cstdint>
#include <iostream>
#include "ProtocolInformation.hpp"
#include "PrintBigNumbers.hpp"
#include "CustomTypes.hpp"

class ProtocolInfo;

extern char* buf;

extern std::unordered_map<uint32_t,ProtocolInfo> protocols;

class Packet {
public:
    uint32_t length = 0;
    uint32_t timestamp = 0;
    uint8_t* data = nullptr;

    ProtocolInfo* protocol;

    uint32_t ptr = 0;

    uint32_t packetCount = 0;
    size_t bufLocation = 0;
public:
    Packet();
    ~Packet();

    bool run(State*);

public:
    bool getBoolean() {
        if((ptr+1)>=length) {return false;}
        return data[ptr++]>0;
    }
    int8_t getByte() {
        if((ptr+1)>=length) {return false;}
        return data[ptr++];
    }
    uint8_t getUByte() {
        if((ptr+1)>=length) {return false;}
        return data[ptr++];
    }
    int16_t getShort() {
        if((ptr+2)>=length) {return false;}
        int16_t val = data[ptr++];
        val <<= 8;
        val |= data[ptr++];
        return val;
    }
    uint16_t getUShort() {
        if((ptr+2)>=length) {return false;}
        uint16_t val = data[ptr++];
        val <<= 8;
        val |= data[ptr++];
        return val;
    }
    int32_t getInt() {
        if((ptr+4)>=length) {return false;}
        int32_t val = data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];

        return val;
    }
    int64_t getLong() {
        if((ptr+8)>=length) {return false;}
        int64_t val = data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];val <<= 8;
        val |= data[ptr++];

        return val;
    }
    float getFloat() {
        if((ptr+4)>=length) {return false;}
        uint32_t a = getLong();
        float val = *((float*)&a);
        return val;
    }
    float getDouble() {
        if((ptr+8)>=length) {return false;}
        uint64_t a = getLong();
        double val = *((double*)&a);
        return val;
    }
    int32_t getVarInt() {
        // Code from https://wiki.vg/Protocol
        int32_t val = 0; 
        int bitOffset = 0;
        uint8_t currentByte;
        while(true) {
            if (bitOffset == 35) return 0;
            currentByte = data[ptr++];
            val |= (currentByte & 0b01111111) << bitOffset;
            bitOffset += 7;
            if((currentByte & 0x80)==0) {break;}
        }
        return val;
    }
    int64_t getVarLong() {
        // Code from https://wiki.vg/Protocol
        int64_t val = 0; 
        int bitOffset = 0;
        uint8_t currentByte;
        do {
            if (bitOffset == 70) return 0;
            currentByte = data[ptr++];
            val |= (currentByte & 0b01111111) << bitOffset;
            bitOffset += 7;
        } while ((currentByte & 0b10000000) != 0);
        return val;
    }
    UUID* getUUID() {
        if((ptr+16)>=length) {return nullptr;}
        UUID* uuid = new UUID();
        memcpy(uuid->uuid,data+ptr,16);
        return uuid;
    }
};