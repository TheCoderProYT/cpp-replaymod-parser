#pragma once

std::string PLAY_PLAYERINFO_Operations[5] = {
  "Add player",
  "Update gamemode",
  "Update ping",
  "Update display name",
  "Remove player"
};

void PLAY_PLAYERINFO() {
  int32_t operation = readVarInt();
  int32_t count = readVarInt();
  fprintf(fileOutput,"Amount of modifications: %i\nModification Type: %s\n",count,PLAY_PLAYERINFO_Operations[operation].c_str());
  for(int i = 0; i < count; i++) {
    fprintf(fileOutput,"%i:\n",i);
    UUID playerUUID = readUUID();
    PlayerInfoObject playerObject;
    switch(operation) {
      case 0:
        playerObject.uuid=playerUUID;
        playerObject.name=readString();
        fprintf(fileOutput,"%s\n",playerObject.toString());
      break;
      case 4:
        fprintf(fileOutput,"Removing player:\n%s\n",playerUUID.toString());
      break;
    }
  }
}
