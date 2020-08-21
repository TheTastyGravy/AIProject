#pragma once
#include <vector>
#include "raymath.h"


// All possible tags an object can have
enum class Tag
{
	Swarmer,
	Leader,
	Spawner,
	Player,
	Wall
};


// Abstract class, primarily for the sake of GameManager
class GameObject
{
	// The class needs to access tags, but they should not be public
	friend class GameManager;
public:
	GameObject(const Vector2& position);
	virtual ~GameObject();

	virtual void update(const float& deltaTime) = 0;
	virtual void draw() = 0;

	// Set the current position of the agent
	void setPos(const Vector2& pos) { position = pos; }
	// Get the current position of the agent
	Vector2 getPos() const { return position; }

	// Add 'tag' to the object, if it doesnt already have it
	void addTag(const Tag tag);
	// Remove 'tag' from the object, if it has it
	void removeTag(const Tag tag);

protected:
	Vector2 position;
private:
	// All the tags this object has. This should only be accessed by GameManager
	std::vector<Tag> tags;
};