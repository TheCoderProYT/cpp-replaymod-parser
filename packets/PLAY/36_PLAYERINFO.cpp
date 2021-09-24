#include "36_PLAYERINFO.hpp"

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
    int counter;
    int value_tmp;
    PlayerInfoObject_Property property_temp;
    switch(operation) {
      case 0:
        for(counter=0;counter<currentState.tabPlayers.size();counter++) {
          if(currentState.tabPlayers[counter].uuid==playerUUID) {
            fprintf(fileOutput,"UUID %s already loaded! Overwriting...\n",playerUUID.toString());
            currentState.tabPlayers.erase(currentState.tabPlayers.begin()+counter);
            break;
          }
        }
        playerObject.uuid=playerUUID;
        playerObject.name=readString();
        for(counter=readVarInt()-1;counter>=0;counter--) {
          property_temp=PlayerInfoObject_Property();
          property_temp.name=readString();
          property_temp.value=readString();
          property_temp.signature=readString(readBoolean());
          playerObject.properties.push_back(property_temp);
        }
        playerObject.gamemode=readVarInt();
        playerObject.ping=readVarInt();
        playerObject.displayName=readString(readBoolean());
        fprintf(fileOutput,"%s",playerObject.toString());
        currentState.tabPlayers.push_back(playerObject);
      break;
      case 1:
        value_tmp=readVarInt();
        for(counter=0;counter<currentState.tabPlayers.size();counter++) {
          if(currentState.tabPlayers[counter].uuid==playerUUID) {
            fprintf(fileOutput,"Set gamemode %s (%i) for UUID %s (%s)",getGamemode(value_tmp).c_str(),value_tmp,playerUUID.toString(),currentState.tabPlayers[counter].name.c_str());
            currentState.tabPlayers[counter].gamemode=value_tmp;
            goto PLAY_PLAYERINFO_LABEL1;
          }
        }
        fprintf(fileOutput,"Error: Cannot set gamemode %s (%i) for UUID %s: UUID not loaded\n",getGamemode(value_tmp).c_str(),value_tmp,playerUUID.toString());
      break;
      case 2:
        value_tmp=readVarInt();
        for(counter=0;counter<currentState.tabPlayers.size();counter++) {
          if(currentState.tabPlayers[counter].uuid==playerUUID) {
            fprintf(fileOutput,"Set ping to %i for UUID %s (%s)",value_tmp,playerUUID.toString(),currentState.tabPlayers[counter].name.c_str());
            currentState.tabPlayers[counter].ping=value_tmp;
            goto PLAY_PLAYERINFO_LABEL1;
          }
        }
        fprintf(fileOutput,"Error: Cannot set gamemode %s (%i) for UUID %s: UUID not loaded\n",getGamemode(value_tmp).c_str(),value_tmp,playerUUID.toString());
      break;
      case 4:
        for(counter=0;counter<currentState.tabPlayers.size();counter++) {
          if(currentState.tabPlayers[counter].uuid==playerUUID) {
            fprintf(fileOutput,"%s",currentState.tabPlayers[counter].toString());
            currentState.tabPlayers.erase(currentState.tabPlayers.begin()+counter);
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
