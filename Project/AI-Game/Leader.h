#pragma once
#include "Agent.h"
#include "Swarmer.h"


// An AI Agent that controls Swarmers and pathfinds between the player and spawners
class Leader :
	public Agent
{
public:
	Leader(Vector2 position, float importance);
	virtual ~Leader();


	virtual void update(float deltaTime);
	virtual void draw();

	void joinSwarm(Swarmer* swarmer);
	void leaveSwarm(Swarmer* swarmer);


	float getImportance() const { return importance; }

	bool getRecruiting() const { return isRecruiting; }
	void setRecruiting(bool isRecruiting) { this->isRecruiting = isRecruiting; }

	// Returns a vector of this leaders swarm
	std::vector<Swarmer*> getSwarm() { return swarm; }
	int getSwarmSize() const { return swarm.size(); }

	// Returns a vector of this leaders formation members
	std::vector<Swarmer*> getFormation() { return formation; }

protected:
	virtual void collisionDetection(float deltaTime, bool& outCollidedX, bool& outCollidedY);


	std::vector<Swarmer*> swarm;
	std::vector<Swarmer*> formation;

private:
	// When set to true and in range of swarmers, they will join this leaders swarm
	bool isRecruiting;
	// Used to impact other boids
	float importance;

	// Used for drawing
	float rotation;
	const float rotationSpeed = 360.0f;
};