#include "Wall.h"
#include "raylib.h"


Wall::Wall(const Vector2& position, const Vector2& size, const Color& color) :
	GameObject(position),
	size(size),
	color(color)
{
	addTag(Tag::Wall);
}

Wall::~Wall()
{
}


void Wall::update(const float& deltaTime)
{
}

void Wall::draw()
{
	// Walls are black squares
	DrawRectangleV(position, size, BLACK);
}