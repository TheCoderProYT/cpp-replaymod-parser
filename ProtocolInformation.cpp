#include "ProtocolInformation.hpp"

ProtocolFunction::ProtocolFunction() {

}

ProtocolFunction::~ProtocolFunction() {

}

void ProtocolFunction::operator()() {
    function();
}

ProtocolMode::ProtocolMode() {

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

ProtocolInfo::~ProtocolInfo() {

}

bool ProtocolInfo::modeDefined(uint32_t a) {
    return modes.count(a)>0;
}

ProtocolMode* ProtocolInfo::operator[](uint32_t a) {
    if(!modeDefined(a)) {return nullptr;}
    return &modes[a];
}

ProtocolInfo protocols = {};