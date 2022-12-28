#include "SkeletonTest.h"


SkeletonTest::SkeletonTest(std::shared_ptr<ObjParams> params) :
    Enemy::Enemy(params)
{
    m_Direction = Direction::backward;
}

void SkeletonTest::Fall(float dt) 
{
    CheckDirectionSetParams(m_AnimationSequences.at("idle"));           // No Fall animation for SkeletonTest yet

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

void SkeletonTest::Attack(float dt) 
{
}


