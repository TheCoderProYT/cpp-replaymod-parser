#include "ProtocolInformation.hpp"

ProtocolFunction::ProtocolFunction() {

}

ProtocolFunction::ProtocolFunction(std::string a, packet_parsing_func b) {
    name=a;
    function=b;
}

ProtocolFunction::~ProtocolFunction() {

}

void ProtocolFunction::operator()() {
    function();
}

ProtocolMode::ProtocolMode() {

}

ProtocolMode::ProtocolMode(std::string a, std::unordered_map<uint32_t,ProtocolFunction> b) {
    name=a;
    functions=b;
}

ProtocolMode::~ProtocolMode() {

}

bool ProtocolMode::functionDefined(uint32_t a) {
    return functions.count(a)>0;
}

ProtocolFunction* ProtocolMode::operator[](uint32_t a) {
    if(!functionDefined(a)) {return nullptr;}
    return &functions[a];
}

ProtocolInfo::ProtocolInfo() {

}

ProtocolInfo::ProtocolInfo(std::string a, std::unordered_map<uint32_t,ProtocolMode> b) {
    name=a;
    modes=b;
}

ProtocolInfo::~ProtocolInfo() {

}

bool ProtocolInfo::modeDefined(uint32_t a) {
    return modes.count(a)>0;
}

ProtocolMode* ProtocolInfo::operator[](uint32_t a) {
    if(!modeDefined(a)) {return nullptr;}
    return &modes[a];
}