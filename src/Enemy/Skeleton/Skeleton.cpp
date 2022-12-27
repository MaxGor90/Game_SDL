#include "Skeleton.h"


Skeleton::Skeleton(std::shared_ptr<ObjParams> params) :
    Enemy::Enemy(params)
{
    m_Direction = Direction::backward;
}

void Skeleton::Fall(float dt) 
{
    CheckDirectionSetParams(m_AnimationSequences.at("idle"));           // No Fall animation for Skeleton yet

    m_RigidBody->UnsetForce();

    m_RigidBody->Update(dt);

    CheckCollisions();

    if (m_Condition == IsIdle)
    {
        Idle(dt);
        return;
    }

    m_Animation->UpdateCycle();
}

void Skeleton::Attack(float dt) 
{
}


