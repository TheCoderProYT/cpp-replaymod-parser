#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <string>
#include <queue>
#include <thread>
#include <fstream>
#include <set>

std::string fileSizeSuffixes[5] = {
  "B",
  "KB",
  "MB",
  "GB",
  "TB"
};
std::string bigNumSuffixes[5] = {
  "",
  "K",
  "M",
  "G",
  "T"
};
char * printFileSize(size_t fileSize) {
  float fsD = (float)fileSize;
  char* buf = new char[256];
  long size = 1;
  for(int i = 0; i < 5; i++) {
    if((fileSize < size*1000) || (i == 4)) {
      sprintf(buf,"%3.2f %s",1000+(fsD/size),fileSizeSuffixes[i].c_str());
      break;
    }
    size*=1000;
  }
  return buf+1;
}
char * printBigNum(size_t num) {
  float nD = (float)num;
  char* buf = new char[256];
  long size = 1;
  for(int i = 0; i < 5; i++) {
    if((num < size*1000) || (i == 4)) {
      sprintf(buf,"%3.2f%s",1000+(nD/size),bigNumSuffixes[i].c_str());
      break;
    }
    size*=1000;
  }
  return buf+1;
}

std::string modeNames[4] = {
  "HANDSHAKE",
  "STATUS",
  "LOGIN",
  "PLAY"
};

int main(int argc, const char** argv) {
  if(argc<2) {
    printf("Usage: %s file\nFile has to be valid .tmcpr file, does not support .mcpr files yet.\n",argv[0]);
    exit(1);
  }
  std::ifstream file(argv[1],std::ios::binary|std::ios::in);
  if(!file.is_open()) {
    printf("Error reading file %s\nDoes the file actually exist?\n",argv[1]);
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

    bzero(data,4000000);
    file.read((char*)data, length);
    tellg+=length;
    tellg+=8;
    int ptr = 0;

    char* buf;

    fprintf(stderr,"\rProcessing: %s / %s [%f%%] (%s packets)",printFileSize(tellg),filesizeFormatted,100*(tellg/(float)filesize),printBigNum(chunks));
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

    printf("[%i] Packet at timestamp %.3fs [length %s] at [%s / %s]: \n",chunks-1,timestamp/1000.0f,printFileSize(length),printFileSize(tellg-8-length),filesizeFormatted);
    printf("Current mode: %i [%s]\n",connectionState,modeNames[connectionState].c_str());
    int packetID = readVarInt();
    printf("Packet ID: 0x%x\n",packetID);
    switch(connectionState) {
      case 2:
        switch(packetID) {
          case 0x2:
            printf("Login packet\n");
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
      fprintf(stderr,"\rInvalid state %i!\n",connectionState);
      exit(1);
    }
    goto fin;
    invalidPacket:;
    if(!unknownPackets.contains(((int64_t)connectionState<<32)|packetID)) {
      unknownPackets.insert(((int64_t)connectionState<<32)|packetID);
    }
    fin:;
    printf("\n");
  }

  // int pointer = 0;
  // //printf("%02x\n",buf[3]);
  // int pSize = 0;
  // while(pointer < filesize) {
  //   uint32_t timestamp = 0;
  //   timestamp+=buf[pointer++];
  //   timestamp*=256;
  //   timestamp+=buf[pointer++];
  //   timestamp*=256;
  //   timestamp+=buf[pointer++];
  //   timestamp*=256;
  //   timestamp+=buf[pointer++];
  //   uint32_t length = 0;
  //   length+=buf[pointer++];
  //   length*=256;
  //   length+=buf[pointer++];
  //   length*=256;
  //   length+=buf[pointer++];
  //   length*=256;
  //   length+=buf[pointer++];
  //   if(length > pSize) {
  //     pSize=length;
  //   }
  //   uint8_t* data = new uint8_t[length];
  //   memcpy(data,buf+pointer,length);
  //   pointer+=length;
  //   fprintf(stderr,"\rProcessing : %s / %s [%f%%]               ",printFileSize(pointer),filesizeFormatted,100*(pointer/(float)filesize));
  //   std::cerr << std::flush;
  //   int ptr = 0;
  //
  //   printf("[%s / %s] Packet at time %ul has size %ul and ",printFileSize(pointer-8),filesizeFormatted,timestamp,length);
  //
  //   int packetID = readVarInt();
  //   printf("packet id of %x\n",packetID);
  //   //printf("\n");
  //   std::cout << std::flush;
  // }
  fprintf(stderr,"\n");
  for(std::set<uint64_t>::iterator i = unknownPackets.begin(); i != unknownPackets.end(); i++) {
    uint64_t x = (*i);
    fprintf(stderr,"%llx %llx\n",x>>32,x&0xffffffff);

  }
}
