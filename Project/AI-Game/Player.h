#pragma once
#include "Agent.h"


class Player :
    public Agent
{
public:
    Player(const Vector2& position, const float& size, const float& attackRadius);
    virtual ~Player();


    virtual void update(const float& deltaTime);
    virtual void draw();

protected:
    const float radius;
    const float size;
};