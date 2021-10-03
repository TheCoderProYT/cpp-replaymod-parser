/*
    CPP-ReplayMod-Parser, a tool for parsing and processing ReplayMod replays
    Copyright (C) 2021 TheCoderPro

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <string>
#include <queue>
#include <thread>
#include <fstream>
#include <set>
#include <cstring>
#include <unordered_map>
#include <chrono>

#include "State.hpp"
#include "PacketStream.hpp"
#include "JSONObject.hpp"
#include "ProtocolInformation.hpp"
#include "PrintBigNumbers.hpp"

extern std::unordered_map<uint32_t,ProtocolInfo> protocols;

State currentState;
PacketStream packetStream;

char* buf = (char*)malloc(65536);

int main(int argc, const char** argv) {
    if(argc<2) {
        printf("Usage: %s folder\nFolder has to be extracted replay folder, this program doesn't support .mcpr files yet.\n",argv[0]);
        exit(0);
    }

    currentState.fileOutput = fopen("out.txt","w");


    size_t filesize;

    std::ifstream file;

    file = std::ifstream(std::string(argv[1])+"/metaData.json",std::ios::binary|std::ios::in);
    if(!file.is_open()) {
        printf("Error reading file %s/metaData.json\nDoes the file actually exist?\n",argv[1]);
        exit(1);
    }

    std::string line;
    getline(file,line);
 
    JSONObject metaDataJSON(line);

    if(!metaDataJSON.hasEntry("protocol")) {
        printf("ERROR: Protocol number not specified in JSON file.\n");
        exit(1);
    }

    uint32_t protocolNumber;

    sscanf(metaDataJSON["protocol"].c_str(),"%i",&protocolNumber);

    if(!protocols.count(protocolNumber)) {
        printf("ERROR: Protocol %i not supported yet!\n",protocolNumber);
        exit(1);
    }

    ProtocolInfo protocol = protocols[protocolNumber];

    printf("Protocol detected: %i (%s)\n",protocolNumber,protocol.name.c_str());

    packetStream.setProtocolNumber(protocolNumber);

    uint32_t replayTotalLength;
    sscanf(metaDataJSON["duration"].c_str(),"%u",&replayTotalLength);
    printf("Total replay duration: %.3f seconds\n",replayTotalLength/1000.0);

    packetStream.loadFile(std::string(argv[1])+"/recording.tmcpr");

    std::set<uint64_t> unknownPackets;

    size_t chunks = 0;

    int printerCounter = 0;

    auto start = std::chrono::steady_clock::now();
    
    while(packetStream.hasMorePackets()) {
        Packet newPacket = *packetStream.next();
        if(++printerCounter==100) {
            printf("Loading: %s / %s [%f%%] at time %ums/%ums [%f%%] %s packets\r",printFileSize(packetStream.currentLocation),packetStream.filesizeString,100*(packetStream.currentLocation/(float)packetStream.filesize),newPacket.timestamp,replayTotalLength,100*(newPacket.timestamp/(float)replayTotalLength),printBigNum(chunks));
            std::cout << std::flush;
            printerCounter=0;
        }
        newPacket.run(&currentState);
        fprintf(currentState.fileOutput,"\n");
        chunks++;
    }

    auto end = std::chrono::steady_clock::now();

    uint64_t length = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    printf("\033[KFinished\n");
    printf("%s packets done in %llu.%09llu seconds\n",printBigNum(chunks),(length/1000000000l),(length%1000000000l));
    fflush(currentState.fileOutput);
    fclose(currentState.fileOutput);
}
