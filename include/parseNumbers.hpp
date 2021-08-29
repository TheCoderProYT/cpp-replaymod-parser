#pragma once

struct UUID {
  uint8_t uuid[16];
  char* toString() {
    char* d = (char*)malloc(64);
    sprintf(d,"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",uuid[0],uuid[1],uuid[2],uuid[3],uuid[4],uuid[5],uuid[6],uuid[7],uuid[8],uuid[9],uuid[10],uuid[11],uuid[12],uuid[13],uuid[14],uuid[15]);
    return d;
  }
};

UUID readUUID() {
  UUID a;
  for(int i = 0; i < 16; i++) {
    a.uuid[i] = data[ptr++];
  }
  return a;
}

int32_t readVarInt() {
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

uint8_t readUnsignedByte() {
  return data[ptr++];
}

std::string readString() {
  std::string o = "";
  int32_t len = readVarInt();
  for(int32_t i = 0; i < len; i++) {
    o+=(char)readUnsignedByte();
  }
  return o;
}
