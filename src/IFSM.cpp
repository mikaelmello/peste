#include "IFSM.hpp"

IFSM::IFSM(GameObject& object) : object(object), pop_requested(false) {}

IFSM::~IFSM() {}
