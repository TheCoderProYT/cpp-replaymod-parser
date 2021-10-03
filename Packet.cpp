#include "Packet.hpp"

Packet::Packet() {

}

Packet::~Packet() {

}

bool Packet::run(State* state_) {
    State state = *state_;

    int protocolID = getVarInt();


    fprintf(state.fileOutput,"[%i] Packet %i at %zu with length %i\nProtocol ID %i:%X - ",timestamp,packetCount,bufLocation,length,state.connectionMode,protocolID);
    
    if(!protocol->modeDefined(state.connectionMode)) {
        fprintf(state.fileOutput,"MODE UNDEFINED\n");
        return false;
    }

    ProtocolMode* pMode = protocol->operator[](state.connectionMode);

    fprintf(state.fileOutput,"%s | ",pMode->name.c_str());
    
    if(!pMode->functionDefined(protocolID)) {
        fprintf(state.fileOutput,"FUNCTION UNDEFINED\n");
        return false;
    }

    ProtocolFunction* pFunc = pMode->operator[](protocolID);

    fprintf(state.fileOutput,"%s\n",pFunc->name.c_str());
    pFunc->operator()(this,state_);
    return true;
}