#include "StateMachine.h"
#include "State.h"
#include "Transition.h"


Vector2 StateMachine::update(Agent* agent, float deltaTime)
{
	// If not set to any state, the machine does nothing
	if (currentState == nullptr)
		return Vector2Zero();

	// Get any triggered transitions
	Transition* trans = currentState->getTriggeredTransition(agent);

	// If a transition was triggered, change state
	if (trans != nullptr)
	{
		currentState->shutdown(agent);
		currentState = trans->getState();
		currentState->setup(agent);
	}

	// Return the state's force
	return currentState->update(agent, deltaTime);
}