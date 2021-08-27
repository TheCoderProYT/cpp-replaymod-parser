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

#include "include/printBigNum.hpp"

FILE* fileOutput;


std::string modeNames[4] = {
  "HANDSHAKE",
  "STATUS",
  "LOGIN",
  "PLAY"
};

void LOGIN_LOGINSUCCESS(uint32_t size, uint8_t* data) {
  fprintf(fileOutput,"Login Success\n");
}

#include "include/protocolInfoStruct.hpp"
#include "protocolInfo.hpp"

int main(int argc, const char** argv) {
  fileOutput = fopen("out.txt", "w");
  if(argc<2) {
    printf("Usage: %s folder\nFolder has to be extracted replay folder, this program doesn't support .mcpr files yet.\nCurrent version support: \n - 1.17.1\n",argv[0]);
    exit(1);
  }
  std::ifstream file(std::string(argv[1])+"/recording.tmcpr",std::ios::binary|std::ios::in);
  if(!file.is_open()) {
    printf("Error reading file %s/recording.tmcpr\nDoes the file actually exist?\n",argv[1]);
    exit(1);
  }
  file.seekg(0,std::ios::end);
  size_t filesize = file.tellg();
  char * filesizeFormatted = printFileSize(filesize);
  file.seekg(0,std::ios::beg);
  uint8_t* data = new uint8_t[4000000];
  int chunks = 0;
  int tellg = 0;

  int connectionState = 2;
  std::set<uint64_t> unknownPackets;

  while(tellg<filesize) {
    chunks++;

    file.read((char*)data, 8);

    uint32_t timestamp = 0;
    timestamp+=data[0];
    timestamp*=256;
    timestamp+=data[1];
    timestamp*=256;
    timestamp+=data[2];
    timestamp*=256;
    timestamp+=data[3];

    uint32_t length = 0;
    length+=data[4];
    length*=256;
    length+=data[5];
    length*=256;
    length+=data[6];
    length*=256;
    length+=data[7];

    for(int i = 0; i < length; i++) {
      data[i]=0;
    }
    file.read((char*)data, length);
    tellg+=length;
    tellg+=8;
    int ptr = 0;

    char* buf;

    printf("Processing: %s / %s [%f%%] (%s packets and %s unknown packet types)\r",printFileSize(tellg),filesizeFormatted,100*(tellg/(float)filesize),printBigNum(chunks),printBigNum(unknownPackets.size()));
    std::cerr << std::flush;
    auto readVarInt = [data,&ptr]() {
      uint32_t v = 0;
     	int bitOffset = 0;
      uint8_t currentByte;
      do {
        if (bitOffset == 35) {
     			printf("VarInt read error!\n");
     			exit(1);
     		}
     		currentByte = data[ptr++];
        v |= (currentByte & 0b01111111) << bitOffset;
    		bitOffset += 7;
      } while ((currentByte & 0b10000000) != 0);
      return v;
    };

    fprintf(fileOutput,"[%i] Packet at timestamp %.3fs [length %s] at [%s / %s]: \n",chunks-1,timestamp/1000.0f,printFileSize(length),printFileSize(tellg-8-length),filesizeFormatted);
    fprintf(fileOutput,"Current mode: %i [%s]\n",connectionState,modeNames[connectionState].c_str());
    int packetID = readVarInt();
    fprintf(fileOutput,"Packet ID: 0x%x\n",packetID);
    switch(connectionState) {
      case 2:
        switch(packetID) {
          case 0x2:
            LOGIN_LOGINSUCCESS(length,data);
            fprintf(fileOutput,"Login Success\n");

          break;
          default:
            goto invalidPacket;
          break;
        }
      break;
      case 3:
        switch(packetID) {
          default:
            goto invalidPacket;
          break;
        }
      break;
      default:
      printf("\rInvalid state %i!\n",connectionState);
      goto error;
    }
    goto fin;
    invalidPacket:;
    if(!unknownPackets.contains(((int64_t)connectionState<<32)|packetID)) {
      unknownPackets.insert(((int64_t)connectionState<<32)|packetID);
    }
    fin:;
    fprintf(fileOutput,"\n");
  }
  printf("\n");
  for(std::set<uint64_t>::iterator i = unknownPackets.begin(); i != unknownPackets.end(); i++) {
    uint64_t x = (*i);
    printf("%llx %llx\n",x>>32,x&0xffffffff);
  }
  error:
  fclose(fileOutput);
  exit(1);
}
