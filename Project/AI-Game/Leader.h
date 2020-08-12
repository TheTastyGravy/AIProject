#pragma once
#include "Agent.h"


// An AI Agent that controls Swarmers and pathfinds between the player and spawners
class Leader :
	public Agent
{
public:
	Leader(Vector2 position);
	virtual ~Leader();


	virtual void update(float deltaTime);
	virtual void draw();



protected:



private:



};