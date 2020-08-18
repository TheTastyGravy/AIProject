#pragma once
#include "Agent.h"


class Player :
    public Agent
{
public:
    Player(Vector2 position, float size, float attackRadius);
    virtual ~Player();


    virtual void update(float deltaTime);
    virtual void draw();

protected:
    float radius;
    float size;
};