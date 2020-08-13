#pragma once
#include "Behaviour.h"


class WanderStateBehav :
	public Behaviour
{
public:
	WanderStateBehav(Vector2 wanderCenter, int wanderRadius, float transitionDistance = 100.0f, float multi = 1.0f);
	virtual ~WanderStateBehav();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, float deltaTime);

protected:
	Vector2 wandCenter;
	int radius;

	// The range an agent can join a recruiting leader
	float maxDist;

	// Used as a multiplier for the wander steering force
	float multi;
};