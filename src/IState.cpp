#include "IState.hpp"

IState::IState(Antagonist& antagonist) : antagonist(antagonist) {}

IState::~IState() {}

void IState::Update(float dt) {}