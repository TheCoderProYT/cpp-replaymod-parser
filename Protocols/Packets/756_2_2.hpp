bool FUNC_756_2_2(Packet* packet, State* state) {
    State state_ = *state;
    FILE* fileOutput = state_.fileOutput;

    UUID* uuid = packet->getUUID();
    if(uuid==nullptr) {return false;}

    fprintf(fileOutput,"UUID: %s\n",uuid->toString().c_str());
    state->connectionMode = 3;
    return true;
}