#pragma once
#include "Behaviour.h"


class FormationStateBehav :
	public Behaviour
{
public:
	FormationStateBehav(Agent* leader, Vector2 offset);
	virtual ~FormationStateBehav();


	virtual Vector2 update(Agent* agent, float deltaTime);

protected:
	Agent* leader;
	Vector2 offset;
};