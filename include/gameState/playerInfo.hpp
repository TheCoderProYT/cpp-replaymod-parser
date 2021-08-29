#pragma once

struct PlayerInfoObject_Property {
  std::string name;
  std::string value;
  std::string signature;
};

struct PlayerInfoObject {
  UUID uuid;
  std::string name;
  std::vector<PlayerInfoObject_Property> properties;
  int32_t gamemode;
  int32_t ping;
  std::string displayName;
  char* toString() {
    char* array = (char*)malloc(2048);
    sprintf(array,"UUID: %s\nUsername: %s\n",uuid.toString(),name.c_str());
    return array;
  }
};
