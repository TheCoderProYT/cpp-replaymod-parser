#include "State.hpp"

State::State() {

}

State::~State() {

}

uint32_t State::getConnectionMode() {
    return connectionMode;
}

void State::setConnectionMode(uint32_t newMode) {
    connectionMode = newMode;
}

