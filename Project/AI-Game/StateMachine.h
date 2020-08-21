#pragma once
#include "Behaviour.h"
#include <memory>
#include <vector>
// Forward definition
class State;
class Transition;


class StateMachine :
	public Behaviour
{
public:
	StateMachine() :
		currentState(nullptr)
	{}
	virtual ~StateMachine() {}


	virtual Vector2 update(Agent* agent, const float& deltaTime);

	void addState(std::shared_ptr<State> state) { states.push_back(state); }
	void setCurrentState(State* state) { currentState = state; }

protected:
	// All states the state machine can be in. The state is actualy found using transitions, but
	// all states should be added to the machine for the sake of memory management.
	std::vector<std::shared_ptr<State>> states;

	// The state machine's current state
	State* currentState;
};