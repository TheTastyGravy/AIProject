#include "Player.h"
#include "Swarmer.h"
#include "GameManager.h"
#include "raylib.h"


Player::Player(Vector2 position, float size, float attackRadius) :
	Agent(position),
	radius(attackRadius),
	size(size)
{
	addTag(Tag::Player);
}

Player::~Player()
{
}


void Player::update(float deltaTime)
{
	// Attack swarmers if the player is holding m1
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		// Get swarmers around the mouse
		for (auto swarmer : GameManager::searchInRadius(Tag::Swarmer, radius, GetMousePosition()))
		{
			((Swarmer*)swarmer)->dealDamage(1);
		}
	}

	// Call any behaviours the agent has
	Agent::update(deltaTime);
}

void Player::draw()
{
	// Draw player base
	DrawRectangleV(Vector2SubtractValue(position, size*0.5f), { size, size }, SKYBLUE);


	// If player is attacking, draw transparent circle in the area
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		DrawCircleV(GetMousePosition(), radius, { 102, 191, 255, 100 });
	}
	// Otherwise, just an outline
	else
	{
		DrawCircleLines(GetMouseX(), GetMouseY(), radius, { 102, 191, 255, 100 });
	}
}