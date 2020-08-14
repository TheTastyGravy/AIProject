#pragma once
#include "GameObject.h"


class Wall :
	public GameObject
{
public:
	Wall(Vector2 position, Vector2 size, Color color = BLACK);
	virtual ~Wall();


	virtual void update(float deltaTime);
	virtual void draw();

	// Return this wall's collision rectangle
	virtual Rectangle getRec() { return { position.x, position.y, size.x, size.y }; }

protected:
	Vector2 size;
	Color color;
};