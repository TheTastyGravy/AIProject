#include "AttackState.h"
#include "GameManager.h"
#include "Leader.h"


AttackState::AttackState(float rangeFromPlayer) :
    range(rangeFromPlayer),
    player()
{
}

AttackState::~AttackState()
{
}


void AttackState::setup(Agent* agent)
{
    // Get the player
    player = (Agent*)GameManager::searchForTag(Tag::Player).front();

    // Swarmers should stop following this
    ((Leader*)agent)->setImportance(0.0f);


    // Set the formation to follow the player
    std::vector<Swarmer*> formation = ((Leader*)agent)->getFormation();
    for (auto itter : formation)
    {
        itter->enterFormation(100.0f, player, { 0, 0 });
    }
}

Vector2 AttackState::update(Agent* agent, float deltaTime)
{
    // Update just keeps the leader at a distance from the player

    // Get the distance from the player
    float dist = Vector2Distance(player->getPos(), agent->getPos());
    Vector2 force;

    // Depending on range, the agent should either flee or seek the player
    if (dist < range)
        force = Vector2Subtract(agent->getPos(), player->getPos());
    else
        force = Vector2Subtract(player->getPos(), agent->getPos());


    // Get steering force as usual
    if (force.x != 0.0f && force.y != 0.0f)
    {
        force = Vector2Normalize(force);
        force = Vector2Scale(force, agent->getMaxSpeed());
    }
    force = Vector2Subtract(force, agent->getVel());
    if (force.x != 0.0f && force.y != 0.0f)
    {
        force = Vector2Normalize(force);
        force = Vector2Scale(force, 100.0f);
    }


    // Scale the force by its difference to the desiered range
    return Vector2Scale(force, (abs(dist - range) / range)*5.0f);
}