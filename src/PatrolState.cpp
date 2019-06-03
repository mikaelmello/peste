#include "PatrolState.hpp"

PatrolState::PatrolState(Antagonist& antagonist) : IState(antagonist) {}

void PatrolState::OnStateEnter() {}

void PatrolState::OnStateExecution() {}

void PatrolState::OnStateExit() {}