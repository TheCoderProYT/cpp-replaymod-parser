#include "PacketStream.hpp"

PacketStream::PacketStream() {
    fileLoaded = false;
    data = (uint8_t*)malloc(8);
}

bool PacketStream::loadFile(std::string filename) {
    fileLoaded = true;
    inputFile = fopen(filename.c_str(),"r");

    fseek(inputFile, 0L, SEEK_END);
    filesize = ftell(inputFile);
    rewind(inputFile);

    filesizeString = printFileSize(filesize);
    fread(data,1,8,inputFile);


    return true;
}

PacketStream::~PacketStream() {
    fclose(inputFile);
}

Packet* PacketStream::next() {
    if(!fileLoaded) {return nullptr;}
    Packet* newPacket = new Packet();
    
    currentLocation+=8;
    newPacket->timestamp|=data[0];
    newPacket->timestamp<<=8;
    newPacket->timestamp|=data[1];
    newPacket->timestamp<<=8;
    newPacket->timestamp|=data[2];
    newPacket->timestamp<<=8;
    newPacket->timestamp|=data[3];

    newPacket->length|=data[4];
    newPacket->length<<=8;
    newPacket->length|=data[5];
    newPacket->length<<=8;
    newPacket->length|=data[6];
    newPacket->length<<=8;
    newPacket->length|=data[7];

    newPacket->data = (uint8_t*)malloc(newPacket->length+8);
    if(newPacket->length+currentLocation==filesize) {
        fread(newPacket->data,1,newPacket->length,inputFile);
    } else {
        fread(newPacket->data,1,newPacket->length+8,inputFile);
        memcpy(data,newPacket->data+newPacket->length,8);
    }
    newPacket->protocol=&protocols[protocolNumber];
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