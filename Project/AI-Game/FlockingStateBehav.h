#pragma once
#include "Behaviour.h"


class FlockingStateBehav :
	public Behaviour
{
public:
	FlockingStateBehav(const float& radius, const float& seperationRadius, const float& wanderRadius, const float& wanderJitter, const float& wanderDist, 
					   const float& seperationMulti = 1.0f, const float& allignmentMulti = 1.0f, const float& cohesionMulti = 1.0f, const float& wanderMulti = 1.0f);
	virtual ~FlockingStateBehav();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, const float& deltaTime);

protected:
	// Flocking variables
	const float radius;
	const float sepRadius;

	const float sepMulti;
	const float allignMulti;
	const float coheMulti;


	// Wander variables
	const float wandRadius;
	const float wandJit;
	const float wandDist;
	const float wandMulti;
};