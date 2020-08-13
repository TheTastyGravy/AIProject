#include "FormationStateBehav.h"
#include "Agent.h"


FormationStateBehav::FormationStateBehav(Agent* leader, Vector2 offset) :
	leader(leader),
	offset(offset)
{
}

FormationStateBehav::~FormationStateBehav()
{
}


Vector2 FormationStateBehav::update(Agent* agent, float deltaTime)
{
	// Find the angle the leader is facing
	float angle = (float)atan2(leader->getVel().y, leader->getVel().x) * (180 / PI);
	Vector2 formationPos = Vector2Rotate(offset, angle);
	// Get the offset relative to the leader
	formationPos = Vector2Add(formationPos, leader->getPos());

	// Get the diference in position to find the heading, then scale to max velocity
	Vector2 seekForce = Vector2Subtract(formationPos, agent->getPos());
	if (seekForce.x != 0.0f && seekForce.y != 0.0f)
	{
		seekForce = Vector2Normalize(seekForce);
		seekForce = Vector2Scale(seekForce, agent->getMaxSpeed());
	}

	// Find the force nessesary to change the velocity
	seekForce = Vector2Subtract(seekForce, agent->getVel());
	// If non-zero, normalise to the base value
	if (seekForce.x != 0.0f && seekForce.y != 0.0f)
	{
		seekForce = Vector2Normalize(seekForce);
		seekForce = Vector2Scale(seekForce, 150.0f);
	}


	// Find the distance from the current position and the formation position
	float distance = Vector2Distance(formationPos, agent->getPos());
	// If the agent is close to the target, start to slow down
	if (distance < 20.0f)
	{
		seekForce = Vector2Scale(seekForce, (distance / 20.0f));
		// Reduce velocity on approach
		seekForce = Vector2Subtract(seekForce, agent->getVel());
	}
	// Apply the force
	return Vector2Scale(seekForce, deltaTime);
}