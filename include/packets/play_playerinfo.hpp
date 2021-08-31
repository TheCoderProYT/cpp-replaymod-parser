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
    int x;
    int y;
    PlayerInfoObject_Property abc;
    switch(operation) {
      case 0:
        for(x=0;x<currentState.tabPlayers.size();x++) {
          if(currentState.tabPlayers[x].uuid==playerUUID) {
            fprintf(fileOutput,"UUID %s already loaded! Overwriting...\n",playerUUID.toString());
            currentState.tabPlayers.erase(currentState.tabPlayers.begin()+x);
            break;
          }
        }
        playerObject.uuid=playerUUID;
        playerObject.name=readString();
        for(x=readVarInt()-1;x>=0;x--) {
          abc=PlayerInfoObject_Property();
          abc.name=readString();
          abc.value=readString();
          abc.signature=readString(readBoolean());
          playerObject.properties.push_back(abc);
        }
        playerObject.gamemode=readVarInt();
        playerObject.ping=readVarInt();
        playerObject.displayName=readString(readBoolean());
        fprintf(fileOutput,"%s",playerObject.toString());
        currentState.tabPlayers.push_back(playerObject);
      break;
      case 1:
        y=readVarInt();
        for(x=0;x<currentState.tabPlayers.size();x++) {
          if(currentState.tabPlayers[x].uuid==playerUUID) {
            fprintf(fileOutput,"Set gamemode %s (%i) for UUID %s (%s)",possibleGamemodes[((y>=0)&&(y<4))?y:4].c_str(),y,playerUUID.toString(),currentState.tabPlayers[x].name.c_str());
            currentState.tabPlayers[x].gamemode=y;
            goto PLAY_PLAYERINFO_LABEL1;
          }
        }
        fprintf(fileOutput,"Error: Cannot set gamemode %s (%i) for UUID %s: UUID not loaded\n",possibleGamemodes[((y>=0)&&(y<4))?y:4].c_str(),y,playerUUID.toString());
      break;
      case 2:
        y=readVarInt();
        for(x=0;x<currentState.tabPlayers.size();x++) {
          if(currentState.tabPlayers[x].uuid==playerUUID) {
            fprintf(fileOutput,"Set ping to %i for UUID %s (%s)",y,playerUUID.toString(),currentState.tabPlayers[x].name.c_str());
            currentState.tabPlayers[x].ping=y;
            goto PLAY_PLAYERINFO_LABEL1;
          }
        }
        fprintf(fileOutput,"Error: Cannot set gamemode %s (%i) for UUID %s: UUID not loaded\n",possibleGamemodes[((y>=0)&&(y<4))?y:4].c_str(),y,playerUUID.toString());
      break;
      case 4:
        for(x=0;x<currentState.tabPlayers.size();x++) {
          if(currentState.tabPlayers[x].uuid==playerUUID) {
            fprintf(fileOutput,"%s",currentState.tabPlayers[x].toString());
            currentState.tabPlayers.erase(currentState.tabPlayers.begin()+x);
            goto PLAY_PLAYERINFO_LABEL1;
          }
        }
        fprintf(fileOutput,"UUID %s not loaded!\n",playerUUID.toString());
      break;
      default:
      break;
    }
    PLAY_PLAYERINFO_LABEL1:
    if(i!=count-1) {fprintf(fileOutput,"\n");}
  }
}
