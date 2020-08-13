#include "FlockingStateBehav.h"
#include "Swarmer.h"
#include "Leader.h"
#include <vector>


FlockingStateBehav::FlockingStateBehav(float radius, float seperationRadius, float seperationMulti, float allignmentMulti, float cohesionMulti) :
	radius(radius),
	sepRadius(seperationRadius),
	sepMulti(seperationMulti),
	allignMulti(allignmentMulti),
	coheMulti(cohesionMulti)
{
	// sepRadius cant be larger than radius
	if (seperationRadius > radius)
		sepRadius = radius;
}

FlockingStateBehav::~FlockingStateBehav()
{
}


Vector2 seperation(std::vector<Swarmer*> swarm, const Agent* agent, float radius, float multi = 1.0f);
Vector2 allignment(const std::vector<Swarmer*>& swarm, const Agent* agent, float multi = 1.0f);
Vector2 cohesion(const std::vector<Swarmer*>& swarm, const Agent* agent, float multi = 1.0f);

Vector2 FlockingStateBehav::update(Agent* agent, float deltaTime)
{
	// Cast the agent to a swarmer
	Swarmer* swarmer = (Swarmer*)agent;
	// Get the swarm from the agent's leader
	std::vector<Swarmer*> swarm = swarmer->getLeader()->getSwarm();

	// Remove any swarmers that are too far away, and this swarmer
	for (int i = 0; i < swarm.size(); i++)
	{
		if (Vector2Distance(swarmer->getPos(), swarm[i]->getPos()) > radius || swarm[i] == swarmer)
		{
			swarm.erase(swarm.begin() + i);
			continue;
		}
	}

	
	// Get and return the flocking force
	if (swarm.size() != 0)
	{
		// Seperation
		Vector2 sepForce = seperation(swarm, agent, sepRadius, sepMulti);
		// Allignment
		Vector2 allignForce = allignment(swarm, agent, allignMulti);
		// Cohesion
		Vector2 coheForce = cohesion(swarm, agent, coheMulti);

		// Get the total force, scaled by delta time
		Vector2 totalForce = Vector2Add(sepForce, Vector2Add(allignForce, coheForce));
		return Vector2Scale(totalForce, deltaTime);
	}
	else
	{
		return Vector2Zero();
	}
}



Vector2 seperation(std::vector<Swarmer*> swarm, const Agent* agent, float radius, float multi)
{
	Vector2 force = { 0, 0 };

	// otherAgents.size cant be used, as not all of them are used
	int c = 0;

	for (auto obj : swarm)
	{
		// Find the distance between the agents
		float dist = Vector2Distance(agent->getPos(), obj->getPos());

		// Add the dirrection from each agent
		// The seperation force uses a smaller radius
		if (dist < radius)
		{
			// Weigh the direction by the inverse of its distance.
			// This results in the force being stronger the closer they are
			Vector2 dir = Vector2Subtract(agent->getPos(), obj->getPos());
			dir = Vector2Normalize(dir);
			dir = Vector2Scale(dir, 1.0f / dist);

			// Add to the total force and increment the count
			force = Vector2Add(force, dir);
			c++;
		}
	}

	// If there were no boids, just return zero
	if (c > 0)
	{
		// Find the mean
		force = Vector2Scale(force, 1.0f / (float)c);

		// Scale all forces to the same magnitude
		if (force.x != 0.0f && force.y != 0.0f)
		{
			force = Vector2Normalize(force);
			force = Vector2Scale(force, 100.0f);
		}
		// Find the stearing force, and weigh it
		return Vector2Scale(Vector2Subtract(force, agent->getVel()), multi);
	}
	else
	{
		return Vector2Zero();
	}
}

Vector2 allignment(const std::vector<Swarmer*>& swarm, const Agent* agent, float multi)
{
	// Use the leader
	Leader* leader = ((Swarmer*)agent)->getLeader();
	Vector2 force = Vector2Scale(leader->getVel(), leader->getImportance());

	// Use the other swarmers
	for (auto obj : swarm)
	{
		// Add velocity to total, scaled by importance
		force = Vector2Add(force, Vector2Scale(obj->getVel(), obj->getImportance()));
	}
	// Find the mean
	force = Vector2Scale(force, 1.0f / (float)swarm.size());

	// Scale all forces to the same magnitude
	if (force.x != 0.0f && force.y != 0.0f)
	{
		force = Vector2Normalize(force);
		force = Vector2Scale(force, 100.0f);
	}
	// Find the stearing force, and weigh it
	return Vector2Scale(Vector2Subtract(force, agent->getVel()), multi);
}

Vector2 cohesion(const std::vector<Swarmer*>& swarm, const Agent* agent, float multi)
{
	// Use the leader
	Leader* leader = ((Swarmer*)agent)->getLeader();
	Vector2 force = Vector2Scale(Vector2Subtract(leader->getPos(), agent->getPos()), leader->getImportance());

	// Use the other swarmers
	for (auto obj : swarm)
	{
		// Find average relative position
		force = Vector2Add(force, Vector2Scale(Vector2Subtract(obj->getPos(), agent->getPos()), obj->getImportance()));
	}
	// Find the mean
	force = Vector2Scale(force, 1.0f / (float)swarm.size());

	// Scale all forces to the same magnitude
	if (force.x != 0.0f && force.y != 0.0f)
	{
		force = Vector2Normalize(force);
		force = Vector2Scale(force, 100.0f);
	}
	// Return the weighted force
	return Vector2Scale(force, multi);
}