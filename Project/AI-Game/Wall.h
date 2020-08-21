#pragma once
#include "GameObject.h"


class Wall :
	public GameObject
{
public:
	Wall(const Vector2& position, const Vector2& size, const Color& color = BLACK);
	virtual ~Wall();


	virtual void update(const float& deltaTime);
	virtual void draw();

	// Return this wall's collision rectangle
	virtual Rectangle getRec() const { return { position.x, position.y, size.x, size.y }; }

protected:
	const Vector2 size;
	const Color color;
};