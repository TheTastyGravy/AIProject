#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
// Forward definition
class Behaviour;


class Agent :
	public GameObject
{
public:
	Agent(Vector2 position, float maxSpeed = 150.0f, float friction = 0.4f);
	virtual ~Agent();


	// Add a behaviour to the agent
	void addBehaviour(std::shared_ptr<Behaviour> behaviour);

	
	// Run the attached behaviours and any other logic the agent has
	virtual void update(float deltaTime);
	// Display the agent
	virtual void draw();


	// Get the current velocity of the agent
	Vector2 getVel() const { return velocity; }
	float getMaxSpeed() const { return maxSpeed; }
	float getFriction() const { return friction; }

protected:
	// shared_ptr is used so multiple agents can use the same behaviour
	std::vector<std::shared_ptr<Behaviour>> behaviours;

	Vector2 velocity;
	float maxSpeed;
	float friction;

	// Used for drawing
	float lastAngle = 0.0f;
};