#pragma once
#include <vector>
#include <memory>
#include "raymath.h"
#include "Transition.h"
// Forward definition
class Agent;


class State
{
public:
	State() {}
	virtual ~State() {}


	virtual void setup(Agent* agent) {};
	virtual Vector2 update(Agent* agent, float deltaTime) = 0;
	virtual void shutdown(Agent* agent) {};

	void addTransition(std::shared_ptr<Transition> transition) { transitions.push_back(transition); }

	// Returns the first transition to trigger, or nullptr if none triggered
	Transition* getTriggeredTransition(Agent* agent)
	{
		for (auto trans : transitions)
		{
			// If it has been triggered, return it
			if (trans->hasTriggered(agent))
				return trans.get();
		}

		// No transition has been triggered
		return nullptr;
	}

protected:
	// List of transitions from this state to other states
	std::vector<std::shared_ptr<Transition>> transitions;
};