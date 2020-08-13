#include "Swarmer.h"
#include "Leader.h"
#include "raylib.h"


Swarmer::Swarmer(Vector2 position, std::shared_ptr<Behaviour> flockingState, int health) :
	Agent(position),
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


void Swarmer::update(float deltaTime)
{
	//behavs are called in agent::update
	Agent::update(deltaTime);

	//an override might not be nessesary
}

void Swarmer::draw()
{
	// Draw swarmers as black dots
	DrawCircleV(position, 2, BLACK);
}


void Swarmer::enterFlocking(float importance, Leader* leader)
{
	// Update importance and leader
	this->importance = importance;
	this->leader = leader;

	// Remove any current behaviours
	behaviours.clear();

	//TODO: update state & set flocking's neibourhood
}

void Swarmer::enterFormation(float importance, Agent* leaderObj, Vector2 offset)
{
	// Update importacne
	this->importance = importance;

	// A swarmer cant enter formation unless it has a ref to a leader
	if (leader == nullptr)
		return;

	// Remove any current behaviours
	behaviours.clear();

	//TODO: create formation state, then set it
}


void Swarmer::dealDamage(int damage)
{
	health -= damage;

	// If health is at 0, kill the swarmer
	if (health <= 0)
	{
		// If this has a leader, remove this from it's swarm
		//if (leader != nullptr)
		//	leader.remove(this);
		
		// Delete this swarmer
		delete this;
	}
}