bool FUNC_756_3_21(Packet* packet, State* state) {
    State state_ = *state;
    FILE* fileOutput = state_.fileOutput;
    int32_t val1 = packet->getInt();
    int32_t val2 = packet->getInt();
    fprintf(fileOutput,"Keepalive ID: 0x%08x%08x\n",val1,val2);
    return true;
}