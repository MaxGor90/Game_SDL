#include "Knight.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Timer.h"
#include "Global.h"
#include "Engine.h"



Knight::Knight(std::shared_ptr<ObjParams> params) : 
    Character::Character(params)
{
    m_Animation = std::make_unique<Animation>();

    m_RigidBody = std::make_unique<RigidBody>();
}

Knight::~Knight()
{
    // Clean();
}




void Knight::Draw()
{
    m_Animation->Draw(m_Transform->m_X, m_Transform->m_Y);
}

//  Main update loop
void Knight::Update(float dt)
{
    switch (m_Condition)
    {
    case isHurt:
        Hurt();
        return;
    case Attacking:
        Attack();
        return;
    case Rolling:
        Roll(dt);
        return;
    case Jumping:
        Jump(dt);
        return;
    case Falling:
        Fall(dt);
        return;
    case Running:
        Run(dt);
        return;
    case IsIdle:
        Idle(dt);
        return;
    case Blocking:
        Block();
        return;
    default:
        break;
    }
}


void Knight::Idle(float dt)
{
    CheckDirectionSetParams(m_AnimationSequences.at("idle"));         // Idle

    m_RigidBody->UnsetForce();
    m_RigidBody->Stop();

    m_ComboState = NO;

    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_A) ||
         Input::getInstance().isKeyDown(SDL_SCANCODE_D) )
    {
        m_Condition = Running;
        Run(dt);
        return;
    }
    
    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_LSHIFT) )
    {
        m_Condition = Rolling;
        Roll(dt);
        return;
    }

    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_SPACE) )
    {
        m_Condition = Jumping;
        m_RigidBody->ApplyForceY(m_JumpSpeedInFrames / TARGET_FPS * UPWARD);
        Jump(dt);
        return;
    }

    if (Input::getInstance().isMouseButtonDown(MOUSE_LB) && m_ComboState == NO)
    {
        m_Condition = Attacking;
        m_ComboState = HIT_1;
        Attack();
        return;
    }

    if ( Input::getInstance().isMouseButtonDown(MOUSE_RB) )
    {
        m_Condition = Blocking;
        Block();
        return;
    }

    CheckCollisions();
    m_Animation->UpdateCycle();
}


void Knight::Run(float dt)
{
    m_RigidBody->UnsetForce();

    if (Input::getInstance().isKeyDown(SDL_SCANCODE_D))
        SetDirection(forward);
    else if (Input::getInstance().isKeyDown(SDL_SCANCODE_A))
        SetDirection(backward);
    else
    {
        m_Condition = IsIdle;
        return;
    }

    CheckDirectionSetParams(m_AnimationSequences.at("run"));                         // Run

    switch (m_Direction)
    {
    case forward:
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames / TARGET_FPS * FORWARD);
        break;
    case backward:
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames / TARGET_FPS * BACKWARD);
        break;
    default:
        break;
    }

    if (!m_Collision.getInstance().CollisionWithMapY(&m_Transform, m_CollisionBox))
    {
        m_Condition = Falling;
        Fall(dt);
        return;
    }

    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_LSHIFT) )
    {
        m_Condition = Rolling;
        Roll(dt);
        return;
    }
    
    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_SPACE) )
    {
        m_Condition = Jumping;
        m_RigidBody->ApplyForceY(m_JumpSpeedInFrames / TARGET_FPS * UPWARD);
        Jump(dt);
        return;
    }

    if ( Input::getInstance().isMouseButtonDown(MOUSE_LB) )
    {
        m_Condition = Attacking;
        m_ComboState = HIT_1;
        Attack();
        return;
    }

    if ( Input::getInstance().isMouseButtonDown(MOUSE_RB) )
    {
        m_Condition = Blocking;
        Block();
        return;
    }

    m_RigidBody->Update(dt);

    CheckCollisions();

    m_Animation->UpdateCycle();
}

void Knight::Roll(float dt)
{

    CheckDirectionSetParams(m_AnimationSequences.at("roll"));                  //  Roll
    
    switch (m_Direction)
    {
    case forward:
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 1.2f / TARGET_FPS * FORWARD);
        break;
    case backward:
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 1.2f / TARGET_FPS * BACKWARD);
        break;
    default:
        break;
    }

    CollisionBoxRecalc();

    if (!m_Collision.getInstance().CollisionWithMapY(&m_Transform, m_CollisionBox))
    {
        m_Animation->SetAnimIsOver();
        m_Condition = Falling;
        return;
    }

    m_RigidBody->Update(dt);

    CheckCollisions();

    if (m_Animation->UpdateSingle())
    {
        m_Condition = IsIdle;
        return;
    }
}

void Knight::Jump(float dt)
{
    CheckDirectionSetParams(m_AnimationSequences.at("jump"));           // Jump

    m_IsInAir = true;
        
    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_A) )
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f / TARGET_FPS * BACKWARD);
        SetDirection(backward);
    }
    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_D) )    
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f / TARGET_FPS * FORWARD);
        SetDirection(forward);
    }

    if (Input::getInstance().isMouseButtonDown(MOUSE_LB) && m_ComboState == NO)
    {
        m_Animation->SetAnimIsOver();
        m_ComboState = HIT_1;
        m_Condition = Attacking;
        Attack();
        return;
    }
    
    m_RigidBody->Update(dt);

    CheckCollisions();

    if (m_RigidBody->getVelocity().m_Y >= 0.0f)
    {
        m_Animation->SetAnimIsOver();
        m_Condition = Falling;
        return;
    }

    m_Animation->UpdateSingle(true);        //  Repeat last frame
    m_RigidBody->UnsetForce();

}

void Knight::Fall(float dt)
{
    CheckDirectionSetParams(m_AnimationSequences.at("fall"));           // Fall

    m_RigidBody->UnsetForce();

    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_A) )
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f / TARGET_FPS * BACKWARD);
        SetDirection(backward);
    }
    if ( Input::getInstance().isKeyDown(SDL_SCANCODE_D) )    
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f  / TARGET_FPS * FORWARD);
        SetDirection(forward);
    }

    if (Input::getInstance().isMouseButtonDown(MOUSE_LB) && m_ComboState == NO)
    {
        m_ComboState = HIT_1;
        m_Condition = Attacking;
        Attack();
    }

    m_RigidBody->Update(dt);

    CheckCollisions();

    if (m_Condition == IsIdle)
    {
        Idle(dt);
        return;
    }

    m_Animation->UpdateCycle();
}

void Knight::Attack()
{
    m_RigidBody->StopX();
    m_RigidBody->UnsetForce();

    switch (m_ComboState)
    {
    case HIT_1:
        CheckDirectionSetParams(m_AnimationSequences.at("attack1"));          //  Attack_1
        if (m_Animation->UpdateSingle(true))        //  Attack ends and pending time for next attack begins
        {
            m_AttackEnds = Timer::getInstance().GetLastTime();
            SetHitScored(false);
        }
        if (m_Animation->IsRepeating()) 
        {
            if (Timer::getInstance().GetLastTime() - m_AttackEnds <= (m_IsInAir? m_TimeBetweenAttacksInAir : m_TimeBetweenAttacks))
            {
                if (Input::getInstance().isMouseButtonDown(MOUSE_LB))
                {
                    m_ComboState = HIT_2;
                    m_Animation->SetAnimIsOver();
                    return;
                }
                //  Any action except attack interrupts combo
                if (Input::getInstance().isKeyDown(SDL_SCANCODE_A)         || 
                    Input::getInstance().isKeyDown(SDL_SCANCODE_D)         || 
                    Input::getInstance().isKeyDown(SDL_SCANCODE_SPACE)     || 
                    Input::getInstance().isKeyDown(SDL_SCANCODE_LSHIFT)    || 
                    Input::getInstance().isMouseButtonDown(MOUSE_RB)) 
                {
                    m_Condition = Falling;          //  Not Idle because you Stop() in Idle and can perform various actions in the air
                    m_Animation->SetAnimIsOver();
                    return;
                }
            } else
            {
                m_Condition = Falling;
                m_Animation->SetAnimIsOver();
                return;
            }
        }    
        break;
    
    case HIT_2:
        CheckDirectionSetParams(m_AnimationSequences.at("attack2"));          //  Attack_2
        if (m_Animation->UpdateSingle(true))        //  Attack ends and pending time for next attack begins
        {    
            m_AttackEnds = Timer::getInstance().GetLastTime();
            SetHitScored(false);
        }
        if (m_Animation->IsRepeating()) 
        {
            if (Timer::getInstance().GetLastTime() - m_AttackEnds <= (m_IsInAir? m_TimeBetweenAttacksInAir : m_TimeBetweenAttacks))
            {
                if (Input::getInstance().isMouseButtonDown(MOUSE_LB))
                {
                    m_ComboState = HIT_3;
                    m_Animation->SetAnimIsOver();
                    return;
                }
                if (Input::getInstance().isKeyDown(SDL_SCANCODE_A) || 
                    Input::getInstance().isKeyDown(SDL_SCANCODE_D) || 
                    Input::getInstance().isKeyDown(SDL_SCANCODE_SPACE) || 
                    Input::getInstance().isKeyDown(SDL_SCANCODE_LSHIFT) || 
                    Input::getInstance().isMouseButtonDown(MOUSE_RB)) 
                {
                    m_Condition = Falling;
                    m_Animation->SetAnimIsOver();
                    return;
                }
            } else
            {
                m_Condition = Falling;
                m_Animation->SetAnimIsOver();
                return;
            }
        }    
        break;
    case HIT_3:
        CheckDirectionSetParams(m_AnimationSequences.at("attack3"));          //  Attack_3
        if (m_Animation->UpdateSingle())
        {    
            m_Condition = Falling;
            SetHitScored(false);
        }
        break;
    
    case NO:
    default:
        m_Condition = Falling;
        break;
    }

    CollisionBoxAtkRecalc();
}

void Knight::Block()
{
    m_RigidBody->Stop();
    m_RigidBody->UnsetForce();

    if (m_HitBlocked)
    {
        m_Animation->SetAnimIsOver();
        SetHitBlocked(false);
        m_HitBlockedEffect = true;
    }
    if (m_HitBlockedEffect)
    {
        CheckDirectionSetParams(m_AnimationSequences.at("blockHit"));
        if (m_Animation->UpdateSingle())
        {
            m_HitBlockedEffect = false;
        }
        return;
    }

    CheckDirectionSetParams(m_AnimationSequences.at("block"));


    if (!Input::getInstance().isMouseButtonDown(MOUSE_RB))
    {
        m_Condition = Falling;
        return;
    }

    CheckCollisions();
    m_Animation->UpdateCycle();
}

void Knight::Hurt()
{
    CheckDirectionSetParams(m_AnimationSequences.at("hurt"));         
    if (m_Animation->UpdateSingle())
        m_Condition = Falling;
}




void Knight::Clean()
{
    TextureManager::getInstance().Drop(m_TextureID);
}





