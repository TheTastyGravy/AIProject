#include "FlockingStateBehav.h"
#include "Swarmer.h"
#include "Leader.h"
#include <vector>
#include <random>
#include <map>


// Used to hold target info unique to each agent
struct UniqueTarget
{
	float timePassed = 0.0f;
	Vector2 target = { 0, 0 };
};
// Map using the agent's pointer as the key
std::map<Agent*, UniqueTarget> agentTargetInfo;


FlockingStateBehav::FlockingStateBehav(float radius, float seperationRadius, float wanderRadius, float wanderJitter, float wanderDist, 
									   float seperationMulti, float allignmentMulti, float cohesionMulti, float wanderMulti) :
	radius(radius),
	sepRadius(seperationRadius),
	sepMulti(seperationMulti),
	allignMulti(allignmentMulti),
	coheMulti(cohesionMulti),
	wandRadius(wanderRadius),
	wandJit(wanderJitter),
	wandDist(wanderDist),
	wandMulti(wanderMulti)
{
	// sepRadius cant be larger than radius
	if (seperationRadius > radius)
		sepRadius = radius;
}

FlockingStateBehav::~FlockingStateBehav()
{
}


// Get a new wander target
void FlockingStateBehav::setup(Agent* agent)
{
	// Create a random number generator for values between -1.0 and 1.0
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0f, 1.0f);


	// Holder to prevent accessing map constantly
	Vector2 target = { 0, 0 };

	// Get a random point on a circle, then scale by radius
	target = { (float)dis(gen), (float)dis(gen) };
	target = Vector2Normalize(target);
	target = Vector2Scale(target, wandRadius);

	// Get random wander, scale by jitter, and add to target
	Vector2 randWander{ (float)dis(gen), (float)dis(gen) };
	randWander = Vector2Normalize(randWander);
	randWander = Vector2Scale(randWander, wandJit);
	target = Vector2Add(target, randWander);

	// Put the point back on the circle
	target = Vector2Normalize(target);
	target = Vector2Scale(target, wandRadius);

	// Find where to move the point
	Vector2 wandDir = agent->getVel();
	if (wandDir.x != 0.0f && wandDir.y != 0.0f)
	{
		wandDir = Vector2Normalize(wandDir);
		wandDir = Vector2Scale(wandDir, wandDist);
	}


	// Move the point to the agent, then ahead of them
	target = Vector2Add(target, agent->getPos());
	target = Vector2Add(target, wandDir);
	// Set the agent's target
	agentTargetInfo[agent].target = target;
}


Vector2 seperation(std::vector<Swarmer*> swarm, const Agent* agent, float radius, float multi = 1.0f);
Vector2 allignment(const std::vector<Swarmer*>& swarm, const Agent* agent, float multi = 1.0f);
Vector2 cohesion(const std::vector<Swarmer*>& swarm, const Agent* agent, float multi = 1.0f);

Vector2 FlockingStateBehav::update(Agent* agent, float deltaTime)
{
	// Cast the agent to a swarmer
	Swarmer* swarmer = (Swarmer*)agent;

	// ----------   SEEK   ----------
	// If the agent to too far away from the leader, seek toward them
	if (Vector2Distance(swarmer->getPos(), swarmer->getLeader()->getPos()) > 100.0f)
	{
		Leader* target = swarmer->getLeader();
		Vector2 desieredVel{ 0, 0 };
		Vector2 force{ 0, 0 };

		// Get the diference in position to find the heading
		desieredVel = Vector2Subtract(target->getPos(), agent->getPos());
		// If non-zero, normalise
		if (desieredVel.x != 0.0f && desieredVel.y != 0.0f)
			desieredVel = Vector2Normalize(desieredVel);
		// Multiply the heading by the max speed to get desiered velocity
		desieredVel = Vector2Scale(desieredVel, agent->getMaxSpeed());


		// Find the force nessesary to change the velocity
		force = Vector2Subtract(desieredVel, agent->getVel());
		// If non-zero, normalise to the base value
		if (force.x != 0.0f && force.y != 0.0f)
		{
			force = Vector2Normalize(force);
			force = Vector2Scale(force, BASE_FORCE);
		}

		// Return final force
		return force;
	}


	// ----------   FLOCKING   ----------
	Vector2 flockingForce;
	{
		// Get the swarm from the agent's leader
		std::vector<Swarmer*> swarm = swarmer->getLeader()->getSwarm();

		// Remove any swarmers that are too far away, and this swarmer
		for (int i = 0; i < swarm.size(); i++)
		{
			if (Vector2Distance(swarmer->getPos(), swarm[i]->getPos()) > radius || swarm[i] == swarmer)
			{
				swarm.erase(swarm.begin() + i);
				// Since i was removed, stay at i
				i--;
				continue;
			}
		}

		// Get the flocking force
		if (swarm.size() != 0)
		{
			// Seperation
			Vector2 sepForce = seperation(swarm, agent, sepRadius, sepMulti);
			// Allignment
			Vector2 allignForce = allignment(swarm, agent, allignMulti);
			// Cohesion
			Vector2 coheForce = cohesion(swarm, agent, coheMulti);

			// Get the total force
			flockingForce = Vector2Add(sepForce, Vector2Add(allignForce, coheForce));
		}
		else
		{
			flockingForce = Vector2Zero();
		}
	}

	// ----------   WANDER   ----------
	Vector2 wandForce;
	{
		// If enough time has passed or the agent is close to the target, find new target, else add to time passed
		if (agentTargetInfo[agent].timePassed > 1.0f ||
			Vector2Distance(agentTargetInfo[agent].target, agent->getPos()) < 5.0f)
		{
			setup(agent);
			agentTargetInfo[agent].timePassed = 0.0f;
		}
		else
		{
			agentTargetInfo[agent].timePassed += deltaTime;
		}


		// Find the displacement between the current and desiered velocity
		wandForce = Vector2Subtract(agentTargetInfo[agent].target, agent->getPos());

		// Scale the point to the base value
		if (wandForce.x != 0.0f && wandForce.y != 0.0f)
		{
			wandForce = Vector2Normalize(wandForce);
			wandForce = Vector2Scale(wandForce, agent->getMaxSpeed());
		}

		// Find the force nessesary to change the velocity
		wandForce = Vector2Subtract(wandForce, agent->getVel());
		// If non-zero, normalise to the base value
		if (wandForce.x != 0.0f && wandForce.y != 0.0f)
		{
			wandForce = Vector2Normalize(wandForce);
			wandForce = Vector2Scale(wandForce, BASE_FORCE);
		}

		// Multiply the force to alter its impact
		wandForce = Vector2Scale(wandForce, wandMulti);
	}


	// Return the combines flocking/wander force
	return Vector2Add(flockingForce, wandForce);
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