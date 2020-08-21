#pragma once
#include "State.h"
// Forward definition
class GameObject;


class FindPlayerState :
	public State
{
public:
	FindPlayerState(const float& Arriveradius, const float& formationRadius);
	virtual ~FindPlayerState();


	virtual void setup(Agent* agent);
	virtual Vector2 update(Agent* agent, const float& deltaTime);
	virtual void shutdown(Agent* agent);

protected:
	GameObject* player;

	const float arriveRadius;
	const float formationRadius;
};