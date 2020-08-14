#include "Wall.h"
#include "raylib.h"


Wall::Wall(Vector2 position, Vector2 size, Color color) :
	GameObject(position),
	size(size),
	color(color)
{
	addTag(Tag::Wall);
}

Wall::~Wall()
{
}


void Wall::update(float deltaTime)
{
}

void Wall::draw()
{
	// Walls are black squares
	DrawRectangleV(position, size, BLACK);
}


