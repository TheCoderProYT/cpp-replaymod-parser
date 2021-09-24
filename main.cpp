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

#include "main.hpp"
#include "State.hpp"
#include "PacketStream.hpp"
#include "JSONObject.hpp"
#include "ProtocolInformation.hpp"
#include "PrintBigNumbers.hpp"

extern ProtocolInfo protocols;

State currentState;
PacketStream packetStream;

FILE* fileOutput;

int main(int argc, const char** argv) {
    if(argc<2) {
        printf("Usage: %s folder\nFolder has to be extracted replay folder, this program doesn't support .mcpr files yet.\n",argv[0]);
        exit(0);
    }

    fileOutput = fopen("out.txt", "w");

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

    if(!protocols.modeDefined(protocolNumber)) {
        printf("ERROR: Protocol %i not supported yet!\n",protocolNumber);
        exit(1);
    }

    ProtocolMode protocol = *protocols[protocolNumber];

    printf("Protocol detected: %i (%s)\n",protocolNumber,protocol.name.c_str());

    packetStream.setProtocolNumber(protocolNumber);

    double replayTotalLength;
    sscanf(metaDataJSON["duration"].c_str(),"%lf",&replayTotalLength);
    printf("Total replay duration: %.3f seconds\n",replayTotalLength/1000.0);

    packetStream.loadFile(std::string(argv[1])+"/recording.tmcpr");

    std::set<uint64_t> unknownPackets;

    size_t chunks = 0;

    while(packetStream.hasMorePackets()) {
        Packet newPacket = *packetStream.next();
        
        printf("Processing: %s / %s [%f%%] at time %.3lfs/%.3lfs (%s packets and %s unknown packet types)\r",printFileSize(packetStream.currentLocation),printFileSize(packetStream.filesize),100*(packetStream.currentLocation/(float)packetStream.filesize),newPacket.timestamp/1000.0,replayTotalLength/1000.0,printBigNum(chunks),printBigNum(unknownPackets.size()));
        std::cout << std::flush;

        fprintf(fileOutput,"\n");
        chunks++;
    }
    printf("\n\nUnknown packet IDs (please report to GitHub): \n");
    std::unordered_map<uint32_t,uint32_t> maximum;
    uint32_t max = 0;
    for(std::set<uint64_t>::iterator i = unknownPackets.begin(); i != unknownPackets.end(); i++) {
        uint64_t x = (*i);
        if(maximum.count(x>>32)==0) {
            maximum[x>>32]=0;
        }
        maximum[x>>32]=((x&0xffffffff)>maximum[x>>32])?x&0xffffffff:maximum[x>>32];
        max=(x>>32)>max?x>>32:max;
    }
    for(int i = 0; i <= max; i++) {
        if(maximum.count(i)==0) {continue;}
        if(!protocol.functionDefined(i)) {
            printf("UNKNOWN%X",i);
        } else {
            printf("%s",(*protocol[i]).name.c_str());
        }
        printf(": ");
        for(int j = 0; j <= maximum[i]; j++) {
            for(uint64_t x : unknownPackets) {
                if((x>>32)!=i) {continue;}
                if((x&0xffffffff)!=j) {continue;}
                printf("%X ",j);
                break;
            }
        }
        printf("\n");
    }
}
