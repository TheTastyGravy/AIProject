#include "KeyboardBehaviour.h"

Vector2 KeyboardBehaviour::update(Agent* agent, float deltaTime)
{
	Vector2 force{ 0, 0 };

	// Check input
	if (IsKeyDown(KEY_UP))
		force.y = -1;
	if (IsKeyDown(KEY_DOWN))
		force.y = 1;
	if (IsKeyDown(KEY_LEFT))
		force.x = -1;
	if (IsKeyDown(KEY_RIGHT))
		force.x = 1;

	// Normalise so diagnals arnt faster, and multiply by the base value
	if (force.x != 0.0f && force.y != 0.0f)
		force = Vector2Normalize(force);
	force = Vector2Scale(force, BASE_FORCE);

	return force;
}