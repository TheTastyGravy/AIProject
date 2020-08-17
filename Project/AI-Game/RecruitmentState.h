#pragma once
#include "State.h"


class RecruitmentState :
    public State
{
public:
    RecruitmentState();
    virtual ~RecruitmentState();


    virtual void setup(Agent* agent);
    virtual Vector2 update(Agent* agent, float deltaTime);
    virtual void shutdown(Agent* agent);
};