#pragma once
#include "State.h"


class FindSpawnerState :
    public State
{
public:
    FindSpawnerState(const float& arrivalRadius);
    virtual ~FindSpawnerState();


    virtual void setup(Agent* agent);
    virtual Vector2 update(Agent* agent, const float& deltaTime);
    virtual void shutdown(Agent* agent);

protected:
    const float arriveRadius;
};