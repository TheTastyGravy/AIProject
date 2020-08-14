#pragma once
#include "Behaviour.h"


class FlockingStateBehav :
	public Behaviour
{
public:
	FlockingStateBehav(float radius, float seperationRadius, float wanderRadius, float wanderJitter, float wanderDist, 
					   float seperationMulti = 1.0f, float allignmentMulti = 1.0f, float cohesionMulti = 1.0f, float wanderMulti = 1.0f);
	virtual ~FlockingStateBehav();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, float deltaTime);

protected:
	// Flocking variables
	float radius;
	float sepRadius;

	float sepMulti;
	float allignMulti;
	float coheMulti;


	// Wander variables
	float wandRadius;
	float wandJit;
	float wandDist;
	float wandMulti;
};