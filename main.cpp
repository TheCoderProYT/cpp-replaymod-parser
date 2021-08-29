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
#include "include/processJSONLine.hpp"

FILE* fileOutput;

uint32_t length;
uint8_t* data;
uint32_t protocolNumber = 0;

uint32_t ptr = 0;
#include "include/parseNumbers.hpp"

#include "include/protocolInfoStruct.hpp"
ProtocolInfo protocol;

int connectionState = 2;

#include "include/gameState/gameState.hpp"
GameState currentState;

#include "include/packets.hpp"
#include "protocolInfo.hpp"

int main(int argc, const char** argv) {
  std::string modeNames[4] = {
    "HANDSHAKE",
    "STATUS",
    "LOGIN",
    "PLAY"
  };
  fileOutput = fopen("out.txt", "w");
  if(argc<2) {
    printf("Usage: %s folder\nFolder has to be extracted replay folder, this program doesn't support .mcpr files yet.\nCurrent version support: \n - 1.17.1\n",argv[0]);
    exit(1);
  }

  size_t filesize;

  std::ifstream file;

  file = std::ifstream(std::string(argv[1])+"/metaData.json",std::ios::binary|std::ios::in);
  if(!file.is_open()) {
    printf("Error reading file %s/metaData.json\nDoes the file actually exist?\n",argv[1]);
    exit(1);
  }

  std::string line;
  getline(file,line);
  //std::cout << line << std::endl;

  std::unordered_map<std::string,std::string> metaDataJSON = processJSONLine(line);
  if(metaDataJSON.count("protocol")==0) {
    printf("ERROR: Protocol number not specified in JSON file.\n");
    exit(1);
  }
  sscanf(metaDataJSON["protocol"].c_str(),"%i",&protocolNumber);

  if(protocols.count(protocolNumber)==0) {
    printf("ERROR: Protocol %i not supported yet!\n",protocolNumber);
    file.close();
    exit(1);
  }

  protocol=protocols[protocolNumber];
  printf("Protocol detected: %i (%s)\n",protocolNumber,protocol.name.c_str());

  if(metaDataJSON.count("duration")==0) {
    printf("ERROR: Protocol number not specified in JSON file.\n");
    exit(1);
  }

  double replayTotalLength;
  sscanf(metaDataJSON["duration"].c_str(),"%lf",&replayTotalLength);
  printf("Total replay duration: %.3f\n",replayTotalLength/1000.0);


  file.close();

  file = std::ifstream(std::string(argv[1])+"/recording.tmcpr",std::ios::binary|std::ios::in);
  if(!file.is_open()) {
    printf("Error reading file %s/recording.tmcpr\nDoes the file actually exist?\n",argv[1]);
    exit(1);
  }
  file.seekg(0,std::ios::end);
  filesize = file.tellg();
  char * filesizeFormatted = printFileSize(filesize);
  file.seekg(0,std::ios::beg);
  data = new uint8_t[4000000];
  int chunks = 0;
  int tellg = 0;

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

    length = 0;
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
    ptr = 0;

    char* buf;

    printf("Processing: %s / %s [%f%%] at time %.3lf/%.3lf (%s packets and %s unknown packet types)\r",printFileSize(tellg),filesizeFormatted,100*(tellg/(float)filesize),timestamp/1000.0,replayTotalLength/1000.0,printBigNum(chunks),printBigNum(unknownPackets.size()));
    std::cerr << std::flush;

    fprintf(fileOutput,"[%i] Packet at timestamp %.3fs [length %s] at [%s / %s]: \n",chunks-1,timestamp/1000.0f,printFileSize(length),printFileSize(tellg-8-length),filesizeFormatted);
    if(protocol.modes.count(connectionState)==0) {
      printf("\nERROR: Invalid state: %i\n",connectionState);
      file.close();
      exit(1);
    }
    fprintf(fileOutput,"Current mode: %i [%s]\n",connectionState,protocol.modes[connectionState].name.c_str());
    int packetID = readVarInt();
    if(protocol.modes[connectionState].functions.count(packetID)==0) {
      fprintf(fileOutput,"Packet ID: 0x%x [Unknown packet]\n\n",packetID);
      if(!unknownPackets.contains(((int64_t)connectionState<<32)|packetID)) {
        unknownPackets.insert(((int64_t)connectionState<<32)|packetID);
      }
      continue;
    }
    fprintf(fileOutput,"Packet ID: 0x%x [%s]\n",packetID,protocol.modes[connectionState].functions[packetID].name.c_str());

    protocol.modes[connectionState].functions[packetID]();

    fprintf(fileOutput,"\n");
  }
  printf("\n\nUnknown packet IDs (please report to GitHub): \n");
  for(std::set<uint64_t>::iterator i = unknownPackets.begin(); i != unknownPackets.end(); i++) {
    uint64_t x = (*i);
    printf("[%s:0x%llx]\n",protocol.modes[x>>32].name.c_str(),x&0xffffffff);
  }
}
