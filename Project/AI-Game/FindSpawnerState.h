#pragma once
#include "State.h"


class FindSpawnerState :
    public State
{
public:
    FindSpawnerState(float arrivalRadius);
    virtual ~FindSpawnerState();


    virtual void setup(Agent* agent);
    virtual Vector2 update(Agent* agent, float deltaTime);
    virtual void shutdown(Agent* agent);

protected:
    float arriveRadius;
};