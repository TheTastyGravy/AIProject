#include "Spawner.h"
#include "Swarmer.h"
#include "raylib.h"
#include "WanderStateBehav.h"

Vector2 randPos(const int posRange, const Vector2& indent);


Spawner::Spawner(const Vector2& position, const std::shared_ptr<Behaviour> flockingState, const int swarmerHealth, const float& timeDelay, const float& radius) :
	GameObject(position),
	timePassed(0.0f),
	timeDelay(timeDelay),
	radius(radius),
	swarmerHealth(swarmerHealth),
	wander(),
	flocking(flockingState)
{
	addTag(Tag::Spawner);

	// Create a wander behaviour for this spawner
	wander = std::make_shared<WanderStateBehav>(position, radius);
}

Spawner::~Spawner()
{
}


void Spawner::update(const float& deltaTime)
{
	timePassed += deltaTime;

	// If enough time has passed, crate a swarmer and reset the timer
	if (timePassed >= timeDelay)
	{
		// Find a radom position around the spawner
		Vector2 pos = randPos((int)radius*2, Vector2SubtractValue(position, radius));
		// Create a new agent at that position
		Agent* swarmer = new Swarmer(pos, flocking, swarmerHealth);
		// Set the active state as wander
		swarmer->addBehaviour(wander);

		// Reset timer
		timePassed = 0.0f;
	}
}

void Spawner::draw()
{
	// Draw spawners as red pentagons
	DrawPoly(position, 5, radius * 0.4f, 0.0f, RED);
}


Vector2 randPos(const int posRange, const Vector2& indent)
{
	return { (float)(rand() % posRange + indent.x), (float)(rand() % posRange + indent.y) };
}