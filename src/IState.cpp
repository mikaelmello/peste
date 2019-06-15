#include "IState.hpp"

IState::IState(GameObject& antagonist)
    : antagonist(antagonist), pop_requested(false) {}

IState::~IState() {}
