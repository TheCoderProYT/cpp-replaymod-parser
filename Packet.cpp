#include "Packet.hpp"

Packet::Packet() {

}

Packet::~Packet() {

}

bool Packet::run(State* state_) {
    State state = *state_;
    fprintf(state.fileOutput,"[%i] Packet %i at %zu with length %i\n",timestamp,packetCount,bufLocation,length);
    int protocolID = getVarInt();

    fprintf(state.fileOutput,"Protocol ID %i:%X - ",state.connectionMode,protocolID);
    fprintf(state.fileOutput,"%s",protocol->modeDefined(state.connectionMode)?protocol->operator[](state.connectionMode)->name.c_str():"MODE UNDEFINED");

    if(!protocol->modeDefined(state.connectionMode)) {
        fprintf(state.fileOutput,"\n");
        return false;
    }
    fprintf(state.fileOutput," | %s\n",protocol->operator[](state.connectionMode)->functionDefined(protocolID)?protocol->operator[](state.connectionMode)->operator[](protocolID)->name.c_str():"FUNCTION UNDEFINED");
    
    if(!protocol->operator[](state.connectionMode)->functionDefined(protocolID)) {
        return false;
    }
    protocol->operator[](state.connectionMode)->operator[](protocolID)->operator()(this,state_);
    return true;
}