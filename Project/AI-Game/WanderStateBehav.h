#pragma once
#include "Behaviour.h"


class WanderStateBehav :
	public Behaviour
{
public:
	WanderStateBehav(const Vector2& wanderCenter, const int wanderRadius, const float& transitionDistance = 100.0f, const float& multi = 1.0f);
	virtual ~WanderStateBehav();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, const float& deltaTime);

protected:
	Vector2 wandCenter;
	const int radius;

	// The range an agent can join a recruiting leader
	const float maxDist;

	// Used as a multiplier for the wander steering force
	const float multi;
};