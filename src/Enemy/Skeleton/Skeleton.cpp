#include "Skeleton.h"


Skeleton::Skeleton(std::shared_ptr<ObjParams> params) :
    Enemy::Enemy(params)
{
    m_Direction = Direction::backward;
    m_RunSpeedInFrames = 2.0f * 60.0f;
}

void Skeleton::Attack(float dt) 
{
    m_RigidBody->StopX();
    m_RigidBody->UnsetForce();

    switch (m_ComboState)
    {
    case HIT_1:
        CheckDirectionSetParams(m_AnimationSequences.at("attack1"));          
        if (m_Animation->UpdateSingle(true))        //  Attack ends and pending time for next attack begins
            m_AttackEnds = SDL_GetTicks64();
        if (m_Animation->IsRepeating())
        {
            isVulnerable = true;
            // if (hurt) idle;
            // if (player is out of sight) idle;

            if ((int)SDL_GetTicks64() - m_AttackEnds >= m_TimeBetweenAttacks)
            {
                m_ComboState = HIT_2;
                m_Animation->SetAnimIsOver();
                return;
            }
        }
        break;
    case HIT_2:
        CheckDirectionSetParams(m_AnimationSequences.at("attack2"));        
        if (m_Animation->UpdateSingle(false))        //  Attack ends and pending time for next attack begins
            m_Condition = Falling;
        break;
    case HIT_3:
    case NO:
    default:
        m_Condition = Falling;
        break;
    }

    CollisionBoxAtkRecalc();
}



