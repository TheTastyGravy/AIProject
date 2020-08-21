#pragma once
#include "GameObject.h"
#include <memory>
// Forward definition
class Behaviour;


// Creates Swarmers within its radius at a regular intival
class Spawner :
	public GameObject
{
public:
	Spawner(const Vector2& position, const std::shared_ptr<Behaviour> flockingState, const int swarmerHealth, const float& timeDelay = 0.5f, const float& radius = 50.0f);
	virtual ~Spawner();


	virtual void update(const float& deltaTime);
	virtual void draw();

protected:
	// The amount of time since the last swarmer was created
	float timePassed;
	// How long to wait between creating swarmers
	const float timeDelay;
	// The area where swarmers can be created, and will wander around
	const float radius;

	// How much health swarmers are created with
	const int swarmerHealth;

	// Behaviours passed to a swarmer when its created
	std::shared_ptr<Behaviour> wander;
	const std::shared_ptr<Behaviour> flocking;
};