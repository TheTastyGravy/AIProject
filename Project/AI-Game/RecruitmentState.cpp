#include "RecruitmentState.h"
#include "Leader.h"


RecruitmentState::RecruitmentState()
{
}

RecruitmentState::~RecruitmentState()
{
}


void RecruitmentState::setup(Agent* agent)
{
    // Start recruiting swarmers
    ((Leader*)agent)->setRecruiting(true);
}

Vector2 RecruitmentState::update(Agent* agent, float deltaTime)
{
    // Stop the agent from moving while recruiting so they stay in range
    Vector2 force = agent->getVel();
    force = Vector2Negate(force);

    return force;
}

void RecruitmentState::shutdown(Agent* agent)
{
    // Stop recruiting swarmers
    ((Leader*)agent)->setRecruiting(false);
}