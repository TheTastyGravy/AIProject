#pragma once
#include "State.h"


class AttackState :
    public State
{
public:
    AttackState(float rangeFromPlayer);
    virtual ~AttackState();


    virtual void setup(Agent* agent);
    virtual Vector2 update(Agent* agent, float deltaTime);

protected:
    Agent* player;
    // How far away from the player should the agent try to be?
    float range;
};