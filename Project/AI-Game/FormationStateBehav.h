#pragma once
#include "Behaviour.h"


class FormationStateBehav :
	public Behaviour
{
public:
	FormationStateBehav(Agent* leader, const Vector2& offset);
	virtual ~FormationStateBehav();


	virtual Vector2 update(Agent* agent, const float& deltaTime);

protected:
	Agent* leader;
	Vector2 offset;
};