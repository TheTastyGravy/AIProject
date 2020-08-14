#pragma once
#include "State.h"
// Forward definition
class GameObject;


class FindPlayerState :
	public State
{
public:
	FindPlayerState(float Arriveradius, float formationRadius);
	virtual ~FindPlayerState();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, float deltaTime);

protected:
	GameObject* player;

	float arriveRadius;
	float formationRadius;
};