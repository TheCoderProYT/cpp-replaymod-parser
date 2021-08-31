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
  std::string printProperties() {
    std::string o;
    for(int i = 0; i < properties.size(); i++) {
      o+=properties[i].name+": "+properties[i].value+"\n";
    }
    return o;
  }
  char* toString() {
    char* array = (char*)malloc(2048);
    sprintf(array,"UUID: %s\nUsername: %s\nGamemode: %s (%i)\nProperties: \n%sPing: %i\n%s%s%s",uuid.toString(),name.c_str(),possibleGamemodes[((gamemode>=0)&&(gamemode<4))?gamemode:4].c_str(),gamemode,printProperties().c_str(),ping,(displayName.length()>0)?"Display Name: ":"",displayName.c_str(),(displayName.length()>0)?"\n":"");
    return array;
  }
};
