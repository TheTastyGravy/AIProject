#pragma once
#include <functional>
// Forward definition
class State;
class Agent;


class Transition
{
public:
	Transition(State* state, std::function<bool(const Agent*)> condition ) :
		state(state),
		condition(condition)
	{}
	virtual ~Transition() {}


	// Return a pointer to the state this would transition to
	State* getState() const { return state; }

	// Has this transition been trigered for 'agent'?
	virtual bool hasTriggered(const Agent* agent) { return condition(agent); }

private:
	State* state;
	std::function<bool(const Agent*)> condition;
};