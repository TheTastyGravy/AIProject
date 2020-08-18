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
	Spawner(Vector2 position, std::shared_ptr<Behaviour> flockingState, int swarmerHealth, float timeDelay = 0.5f, float radius = 50.0f);
	virtual ~Spawner();


	virtual void update(float deltaTime);
	virtual void draw();

protected:
	// The amount of time since the last swarmer was created
	float timePassed;
	// How long to wait between creating swarmers
	float timeDelay;
	// The area where swarmers can be created, and will wander around
	float radius;

	// How much health swarmers are created with
	int swarmerHealth;

	// Behaviours passed to a swarmer when its created
	std::shared_ptr<Behaviour> wander;
	std::shared_ptr<Behaviour> flocking;
};