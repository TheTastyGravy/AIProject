#pragma once
#include "raymath.h"
// Forward definition
class Agent;


class Behaviour
{
public:
	Behaviour(const float& speed = 100.0f) :
		BASE_FORCE(speed)
	{}
	virtual ~Behaviour() {}


	virtual void setup(Agent* agent) {};
	virtual Vector2 update(Agent* agent, const float& deltaTime) = 0;

protected:
	// The value behaviours will be normalised to
	const float BASE_FORCE;
};