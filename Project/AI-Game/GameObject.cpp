#include "GameObject.h"
#include "GameManager.h"


GameObject::GameObject(Vector2 position) :
	position(position)
{
	// Add this object to the pool
	GameManager::addToPool(this);
}

GameObject::~GameObject()
{
	// Remove this object from the pool
	GameManager::removeFromPool(this);
}


void GameObject::addTag(Tag tag)
{
	// Look at this object's tags
	for (auto& var : tags)
	{
		// The tag is alraedy assigned, so return
		if (var == tag)
			return;
	}

	// The tag isnt already assigned, so add it
	tags.push_back(tag);
}

void GameObject::removeTag(Tag tag)
{
	// Find the tag and remove it
	for (int i = 0; i < tags.size(); i++)
	{
		if (tags[i] == tag)
			tags.erase(tags.begin() + i);
	}
}