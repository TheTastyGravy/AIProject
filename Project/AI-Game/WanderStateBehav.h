#pragma once
#include "Behaviour.h"


class WanderStateBehav :
	public Behaviour
{
public:
	WanderStateBehav(Vector2 wanderCenter, float wanderRadius, float multi = 1.0f);
	virtual ~WanderStateBehav();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, float deltaTime);

protected:
	Vector2 wandCenter;
	float radius;

	// Used as a multiplier for the wander steering force
	float multi;

};