#pragma once

#include <string>
#include <unordered_map>

class JSONObject {
private:
    std::unordered_map<std::string,std::string> data;
public:
    JSONObject(std::string);
    ~JSONObject();

    bool hasEntry(std::string);
    std::string getEntry(std::string);
    std::string operator[](std::string);
};