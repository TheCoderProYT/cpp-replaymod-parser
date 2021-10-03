#pragma once
#include <cstdint>
#include <string>

struct UUID {
    uint8_t uuid[16];
    UUID() {}
    ~UUID() {}
    std::string toString() {
        char a[40] = {0};
        for(int i = 0; i < 16; i++) {
            sprintf(a+i*2,"%02x",*(uuid+i));
        }
        return std::string(a);
    }
    bool operator==(const UUID other) {
        for(int i = 0; i < 16; i++) {
            if(other.uuid[i]!=uuid[i]) {return false;}
        }
        return true;
    }
};

struct User {
    UUID uuid;
    std::string name;
};