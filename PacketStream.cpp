#include "PacketStream.hpp"

PacketStream::PacketStream() {
    fileLoaded = false;
}

bool PacketStream::loadFile(std::string filename) {
    fileLoaded = true;
    inputFile = std::ifstream(filename,std::ios::binary|std::ios::in);
    if(!inputFile.is_open()) {
        printf("Error reading file %s\nDoes the file actually exist?\n",filename.c_str());
        return false;
    }
    inputFile.seekg(0,std::ios::end);
    filesize = inputFile.tellg();
    inputFile.seekg(0,std::ios::beg);
    return true;
}

PacketStream::~PacketStream() {
    inputFile.close();
}

Packet* PacketStream::next() {
    if(!fileLoaded) {return nullptr;}
    Packet* newPacket = new Packet();
    
    uint8_t buf1[8] = {0};

    newPacket->bufLocation=currentLocation;

    inputFile.read((char*)buf1,8);
    currentLocation+=8;

    newPacket->timestamp+=buf1[0];
    newPacket->timestamp*=256;
    newPacket->timestamp+=buf1[1];
    newPacket->timestamp*=256;
    newPacket->timestamp+=buf1[2];
    newPacket->timestamp*=256;
    newPacket->timestamp+=buf1[3];

    newPacket->length+=buf1[4];
    newPacket->length*=256;
    newPacket->length+=buf1[5];
    newPacket->length*=256;
    newPacket->length+=buf1[6];
    newPacket->length*=256;
    newPacket->length+=buf1[7];

    newPacket->data = (uint8_t*)malloc(newPacket->length);
    inputFile.read((char*)newPacket->data, newPacket->length);
    currentLocation+=newPacket->length;
    newPacket->packetCount=packetIDs++;

    return newPacket;
}

bool PacketStream::hasMorePackets() {
    return (currentLocation!=filesize);
}

void PacketStream::setProtocolNumber(uint32_t val) {
    protocolNumber=val;
}