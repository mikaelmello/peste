#include "IFSM.hpp"

IFSM::IFSM(GameObject& antagonist)
    : antagonist(antagonist), pop_requested(false) {}

IFSM::~IFSM() {}
