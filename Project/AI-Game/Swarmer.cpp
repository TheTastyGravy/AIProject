#include "Swarmer.h"
#include "Leader.h"
#include "raylib.h"
#include "FormationStateBehav.h"


Swarmer::Swarmer(Vector2 position, std::shared_ptr<Behaviour> flockingState, int health) :
	Agent(position, 200.0f),
	importance(0.0f),
	leader(nullptr),
	health(health),
	flocking(flockingState)
{
	addTag(Tag::Swarmer);
}

Swarmer::~Swarmer()
{
}


void Swarmer::draw()
{
	// Draw swarmers as black dots
	DrawCircleV(position, 2, BLACK);
}


void Swarmer::enterFlocking(float importance, Leader* leader)
{
	// Update importance and leader, and join its swarm
	this->importance = importance;
	this->leader = leader;
	leader->joinSwarm(this);
	// Remove any current behaviours and set current to flocking
	behaviours.clear();
	addBehaviour(flocking);
}

void Swarmer::enterFormation(float importance, Agent* leaderObj, Vector2 offset)
{
	// A swarmer cant enter formation unless it has a ref to a leader
	if (leader == nullptr)
		return;

	// Update importacne
	this->importance = importance;
	// Remove any current behaviours
	behaviours.clear();

	// Add a new formation state to be the current behaviour
	addBehaviour(std::make_shared<FormationStateBehav>(leaderObj, offset));
}


void Swarmer::dealDamage(int damage)
{
	health -= damage;

	// If health is at 0, kill the swarmer
	if (health <= 0)
	{
		// If this has a leader, remove this from it's swarm
		if (leader != nullptr)
			leader->leaveSwarm(this);
		
		// Delete this swarmer
		delete this;
	}
}