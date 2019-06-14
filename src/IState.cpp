#include "IState.hpp"

IState::IState(GameObject& antagonist) : antagonist(antagonist) {}

IState::~IState() {}

void IState::Update(float dt) {}

void IState::OnStateEnter() {}

void IState::OnStateExecution() {}

void IState::OnStateExit() {}