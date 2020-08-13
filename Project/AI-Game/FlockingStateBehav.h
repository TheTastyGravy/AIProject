#pragma once
#include "Behaviour.h"


class FlockingStateBehav :
	public Behaviour
{
public:
	FlockingStateBehav(float radius, float seperationRadius, float seperationMulti = 1.0f, float allignmentMulti = 1.0f, float cohesionMulti = 1.0f);
	virtual ~FlockingStateBehav();


	virtual Vector2 update(Agent* agent, float deltaTime);

protected:
	float radius;
	float sepRadius;

	float sepMulti;
	float allignMulti;
	float coheMulti;
};