#include "Agent.h"
#include "Behaviour.h"
#include "raylib.h"


Agent::Agent(Vector2 position, float maxSpeed, float friction) :
	GameObject(position),
	behaviours(),
	velocity({ 0, 0 }),
	maxSpeed(maxSpeed),
	friction(friction)
{
}

Agent::~Agent()
{
}


void Agent::addBehaviour(std::shared_ptr<Behaviour> behaviour)
{
	behaviours.push_back(behaviour);
	// Execute any setup behaviour
	behaviour->setup(this);
}


void Agent::update(float deltaTime)
{
	// Holds the total force from behaviours
	Vector2 force{ 0, 0 };

	// Call update on each behaviour
	for (auto behav : behaviours)
	{
		Vector2 behavForce = behav->update(this, deltaTime);
		// Get the cumulative force
		force = Vector2Add(force, behavForce);
	}


	// Add force * delta time
	velocity = Vector2Add(velocity, Vector2Scale(force, deltaTime));

	if (Vector2Length(velocity) > maxSpeed)
	{
		velocity = Vector2Normalize(velocity);
		velocity = Vector2Scale(velocity, maxSpeed);
	}

	// Add velocity * dalta time
	position = Vector2Add(position, Vector2Scale(velocity, deltaTime));
	// Apply friction
	velocity = Vector2Scale(velocity, 1.0f - (friction * deltaTime));
}

void Agent::draw()
{
	const float SIZE = 10.0f;

	//DrawRectangle((int)position.x-SIZE/2, (int)position.y-SIZE/2, SIZE, SIZE, RED);
	//DrawPoly(position, 3, SIZE/2, atan2(velocity.y, velocity.x)*(180/PI)+33.33f, RED);
	
	// Find the corners for an isosceles triangle pointing in the direction of movement
	if (velocity.x != 0.0f || velocity.y != 0.0f)
		lastAngle = (float)atan2(velocity.y, velocity.x) * (180 / PI);

	Vector2 v1 = { SIZE/2, 0 };
	v1 = Vector2Rotate(v1, lastAngle);
	v1 = Vector2Add(v1, position);

	Vector2 v2 = { -SIZE/2, -(SIZE/2 - 0.5f) };
	v2 = Vector2Rotate(v2, lastAngle);
	v2 = Vector2Add(v2, position);

	Vector2 v3 = { -SIZE/2, +(SIZE/2 - 0.5f) };
	v3 = Vector2Rotate(v3, lastAngle);
	v3 = Vector2Add(v3, position);

	DrawTriangle(v1, v2, v3, RED);
}