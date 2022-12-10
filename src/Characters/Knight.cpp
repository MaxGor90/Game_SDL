#include "Knight.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Timer.h"
#include "Global.h"
#include "Engine.h"
#include "Knight_params.h"



Knight::Knight(std::shared_ptr<ObjParams> params) : 
    Character::Character(params)
{
    m_Animation = std::make_unique<Animation>();
    m_CollisionBox = std::make_shared<SDL_Rect>(SDL_Rect{0,0,0,0});
    m_CollisionBoxAtk = std::make_shared<SDL_Rect>(SDL_Rect{0,0,0,0});

    SetParams(m_TextureID, FALL_FRAME_ROW, FALL_FRAME_COUNT, FALL_FRAME_TIME, SDL_FLIP_NONE, FALL_FRAME_START);
    
    m_RigidBody = std::make_unique<RigidBody>();
    
    m_Collision = Collision::GetInstance();
}

Knight::~Knight()
{
    Clean();
}

void Knight::SetParams( const std::string& textureID, int spriteRow, int frameCount, int frameTime, 
                        SDL_RendererFlip flip, int startFrame)
{
    m_Animation->SetParams(textureID, spriteRow, frameCount, frameTime, flip, startFrame);
    m_Width = TextureManager::getInstance()->GetTextureWidth(textureID);
    m_Height = TextureManager::getInstance()->GetTextureHeight(textureID);

    CollisionBoxRecalc();
}

bool Knight::CheckCollisions()
{
    bool isCollision {false};

    //  Calculate position where we are going to go

    m_Transform->Translate(m_RigidBody->getPosition());
    
    PositionRecalc();

    //  Check if there will still be collision in that position
    if (m_Collision->GetInstance()->CollisionWithMapX(&m_Transform, m_CollisionBox))
    {
        //  If yes - stop moving and position is getting corrected by CollisionWithMapX
        //  Recalc m_pos and collision box 
        m_RigidBody->StopX();
        PositionRecalc();
        isCollision = true;
    }

    //  Same for collsiion with Y                                      
    if (m_Collision->GetInstance()->CollisionWithMapY(&m_Transform, m_CollisionBox))
    {
        m_RigidBody->StopY();
        PositionRecalc();
        isCollision = true;
        
        if (m_Condition == Falling)
        {
            m_Condition = IsIdle;
            m_IsInAir = false;
        }
    } else if (m_Condition == IsIdle)
    {
        m_Condition = Falling;
        m_IsInAir = true;
    }

    return isCollision;
}

//  Recalc (with bounds check) player's position and its collision box
void Knight::PositionRecalc()
{
    CheckPosition();
    CollisionBoxRecalc();
    
    m_Position->X = m_Transform->m_X + m_Width / 2;
    m_Position->Y = m_Transform->m_Y + m_Height / 2;
}

//  Sets passed animation parameters depending on current direction
void Knight::CheckDirectionSetParams(int spriteRow, int frameCount, int frameTime, int startFrame)
{
    switch (m_Direction)
    {
    case forward:
        SetParams(m_TextureID, spriteRow, frameCount, frameTime, SDL_FLIP_NONE, startFrame);        
        break;
    case backward:
        SetParams(m_TextureID, spriteRow, frameCount, frameTime, SDL_FLIP_HORIZONTAL, startFrame);
        break;
    default:
        break;
    }
}

void Knight::CheckPosition()
{
    //  Check if Collision Box is going out of screen (and hence gameMap) bounds
    //  If yes - keep it within bounds
    if (m_Transform->m_X < -COLLISION_X)  m_Transform->m_X = -COLLISION_X;
    if (m_Transform->m_Y < -COLLISION_Y)  m_Transform->m_Y = -COLLISION_Y;
    if (m_Transform->m_X > Engine::getInstance()->getMap()->getMapLength() - m_Width + COLLISION_X)  
        m_Transform->m_X = Engine::getInstance()->getMap()->getMapLength() - m_Width + COLLISION_X;
    if (m_Transform->m_Y > SCREEN_HEIGHT - m_Height)  m_Transform->m_Y = SCREEN_HEIGHT - m_Height;
}

//  Updates player's collision box
void Knight::CollisionBoxRecalc()
{
    m_CollisionBox->x = (int)m_Transform->m_X + COLLISION_X;
    m_CollisionBox->y = (int)m_Transform->m_Y + ((m_Condition == Rolling)? COLLISION_ROLL_Y : COLLISION_Y);         
    m_CollisionBox->w = COLLISION_WIDTH;
    m_CollisionBox->h = ((m_Condition == Rolling)? COLLISION_ROLL_HEIGHT : COLLISION_HEIGHT);                                 
}

//  Updates player's attack collision box
void Knight::CollisionBoxAtkRecalc()
{
    switch (m_ComboState)
    {
    case HIT_1:
        switch (m_Direction)
        {
        case forward:
            m_CollisionBoxAtk->x = (int)m_Transform->m_X + COLLISION_ATK1_X;
            m_CollisionBoxAtk->y = (int)m_Transform->m_Y + COLLISION_ATK1_Y;         
            m_CollisionBoxAtk->w = COLLISION_ATK1_WIDTH;
            m_CollisionBoxAtk->h = COLLISION_ATK1_HEIGHT;
            return;
        case backward:
            m_CollisionBoxAtk->x = (int)m_Transform->m_X + m_Width - COLLISION_ATK1_X - COLLISION_ATK1_WIDTH;
            m_CollisionBoxAtk->y = (int)m_Transform->m_Y + COLLISION_ATK1_Y;         
            m_CollisionBoxAtk->w = COLLISION_ATK1_WIDTH;
            m_CollisionBoxAtk->h = COLLISION_ATK1_HEIGHT;
            return;
        }
        return;
    case HIT_2:
        switch (m_Direction)
        {
        case forward:
            m_CollisionBoxAtk->x = (int)m_Transform->m_X + COLLISION_ATK2_X;
            m_CollisionBoxAtk->y = (int)m_Transform->m_Y + COLLISION_ATK2_Y;         
            m_CollisionBoxAtk->w = COLLISION_ATK2_WIDTH;
            m_CollisionBoxAtk->h = COLLISION_ATK2_HEIGHT;
            return;
        case backward:
            m_CollisionBoxAtk->x = (int)m_Transform->m_X + m_Width - COLLISION_ATK2_X - COLLISION_ATK2_WIDTH;
            m_CollisionBoxAtk->y = (int)m_Transform->m_Y + COLLISION_ATK2_Y;         
            m_CollisionBoxAtk->w = COLLISION_ATK2_WIDTH;
            m_CollisionBoxAtk->h = COLLISION_ATK2_HEIGHT;
            return;
        }
        return;
    case HIT_3:
        switch (m_Direction)
        {
        case forward:
            m_CollisionBoxAtk->x = (int)m_Transform->m_X + COLLISION_ATK3_X;
            m_CollisionBoxAtk->y = (int)m_Transform->m_Y + COLLISION_ATK3_Y;         
            m_CollisionBoxAtk->w = COLLISION_ATK3_WIDTH;
            m_CollisionBoxAtk->h = COLLISION_ATK3_HEIGHT;
            return;
        case backward:
            m_CollisionBoxAtk->x = (int)m_Transform->m_X + m_Width - COLLISION_ATK3_X - COLLISION_ATK3_WIDTH;
            m_CollisionBoxAtk->y = (int)m_Transform->m_Y + COLLISION_ATK3_Y;         
            m_CollisionBoxAtk->w = COLLISION_ATK3_WIDTH;
            m_CollisionBoxAtk->h = COLLISION_ATK3_HEIGHT;
            return;
        }
        return;
    case NO:
    //  Fallthrough
    default:
        return;
    }
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
    case Attacking:
        Attack(dt);
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
    CheckDirectionSetParams(IDLE_FRAME_ROW, IDLE_FRAME_COUNT, IDLE_FRAME_TIME, IDLE_FRAME_START);         // Idle

    m_RigidBody->UnsetForce();
    m_RigidBody->Stop();

    m_ComboState = NO;

    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_A) )
    {
        SetDirection(backward);
        m_Condition = Running;
        Run(dt);
        return;
    }
    
    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_D) )
    {
        SetDirection(forward);
        m_Condition = Running;
        Run(dt);
        return;
    }

    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_LSHIFT) )
    {
        m_Condition = Rolling;
        Roll(dt);
        return;
    }

    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_SPACE) )
    {
        m_Condition = Jumping;
        m_RigidBody->ApplyForceY(m_JumpSpeedInFrames / TARGET_FPS * UPWARD);
        Jump(dt);
        return;
    }

    if (Input::getInstance()->isMouseButtonDown(MOUSE_LB) && m_ComboState == NO)
    {
        m_Condition = Attacking;
        m_ComboState = HIT_1;
        Attack(dt);
        return;
    }

    if ( Input::getInstance()->isMouseButtonDown(MOUSE_RB) )
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

    if (Input::getInstance()->isKeyDown(SDL_SCANCODE_D))
        SetDirection(forward);
    else if (Input::getInstance()->isKeyDown(SDL_SCANCODE_A))
        SetDirection(backward);
    else
    {
        m_Condition = IsIdle;
        return;
    }

    CheckDirectionSetParams(RUN_FRAME_ROW, RUN_FRAME_COUNT, RUN_FRAME_TIME, RUN_FRAME_START);                         // Run

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

    if (!m_Collision->GetInstance()->CollisionWithMapY(&m_Transform, m_CollisionBox))
    {
        m_Condition = Falling;
        Fall(dt);
        return;
    }

    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_LSHIFT) )
    {
        m_Condition = Rolling;
        Roll(dt);
        return;
    }
    
    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_SPACE) )
    {
        m_Condition = Jumping;
        m_RigidBody->ApplyForceY(m_JumpSpeedInFrames / TARGET_FPS * UPWARD);
        Jump(dt);
        return;
    }

    if ( Input::getInstance()->isMouseButtonDown(MOUSE_LB) )
    {
        m_Condition = Attacking;
        m_ComboState = HIT_1;
        Attack(dt);
        return;
    }

    if ( Input::getInstance()->isMouseButtonDown(MOUSE_RB) )
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

    CheckDirectionSetParams(ROLL_FRAME_ROW, ROLL_FRAME_COUNT, ROLL_FRAME_TIME, ROLL_FRAME_START);                  //  Roll
    
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

    if (!m_Collision->GetInstance()->CollisionWithMapY(&m_Transform, m_CollisionBox))
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
    CheckDirectionSetParams(JUMP_FRAME_ROW, JUMP_FRAME_COUNT, JUMP_FRAME_TIME, JUMP_FRAME_START);           // Jump

    m_IsInAir = true;
        
    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_A) )
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f / TARGET_FPS * BACKWARD);
        SetDirection(backward);
    }
    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_D) )    
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f / TARGET_FPS * FORWARD);
        SetDirection(forward);
    }

    if (Input::getInstance()->isMouseButtonDown(MOUSE_LB) && m_ComboState == NO)
    {
        m_Animation->SetAnimIsOver();
        m_ComboState = HIT_1;
        m_Condition = Attacking;
        Attack(dt);
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
    CheckDirectionSetParams(FALL_FRAME_ROW, FALL_FRAME_COUNT, FALL_FRAME_TIME, FALL_FRAME_START);           // Fall

    m_RigidBody->UnsetForce();

    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_A) )
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f / TARGET_FPS * BACKWARD);
        SetDirection(backward);
    }
    if ( Input::getInstance()->isKeyDown(SDL_SCANCODE_D) )    
    {
        m_RigidBody->ApplyForceX(m_RunSpeedInFrames * 0.8f  / TARGET_FPS * FORWARD);
        SetDirection(forward);
    }

    if (Input::getInstance()->isMouseButtonDown(MOUSE_LB) && m_ComboState == NO)
    {
        m_ComboState = HIT_1;
        m_Condition = Attacking;
        Attack(dt);
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

void Knight::Attack(float dt)
{
    m_RigidBody->StopX();
    m_RigidBody->UnsetForce();

    switch (m_ComboState)
    {
    case HIT_1:
        CheckDirectionSetParams(ATK1_FRAME_ROW, ATK1_FRAME_COUNT, ATK1_FRAME_TIME, ATK1_FRAME_START);          //  Attack_1
        if (m_Animation->UpdateSingle(true))        //  Attack ends and pending time for next attack begins
            m_AttackEnds = SDL_GetTicks64();
        if (m_Animation->IsRepeating()) 
        {
            if ((int)SDL_GetTicks64() - m_AttackEnds <= (m_IsInAir? m_TimeBetweenAttacksInAir : m_TimeBetweenAttacks))
            {
                if (Input::getInstance()->isMouseButtonDown(MOUSE_LB))
                {
                    m_ComboState = HIT_2;
                    m_Animation->SetAnimIsOver();
                    return;
                }
                //  Any action except attack interrupts combo
                if (Input::getInstance()->isKeyDown(SDL_SCANCODE_A)         || 
                    Input::getInstance()->isKeyDown(SDL_SCANCODE_D)         || 
                    Input::getInstance()->isKeyDown(SDL_SCANCODE_SPACE)     || 
                    Input::getInstance()->isKeyDown(SDL_SCANCODE_LSHIFT)    || 
                    Input::getInstance()->isMouseButtonDown(MOUSE_RB)) 
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
        CheckDirectionSetParams(ATK2_FRAME_ROW, ATK2_FRAME_COUNT, ATK2_FRAME_TIME, ATK2_FRAME_START);          //  Attack_2
        if (m_Animation->UpdateSingle(true))        //  Attack ends and pending time for next attack begins
            m_AttackEnds = SDL_GetTicks64();
        if (m_Animation->IsRepeating()) 
        {
            if ((int)SDL_GetTicks64() - m_AttackEnds <= (m_IsInAir? m_TimeBetweenAttacksInAir : m_TimeBetweenAttacks))
            {
                if (Input::getInstance()->isMouseButtonDown(MOUSE_LB))
                {
                    m_ComboState = HIT_3;
                    m_Animation->SetAnimIsOver();
                    return;
                }
                if (Input::getInstance()->isKeyDown(SDL_SCANCODE_A) || 
                    Input::getInstance()->isKeyDown(SDL_SCANCODE_D) || 
                    Input::getInstance()->isKeyDown(SDL_SCANCODE_SPACE) || 
                    Input::getInstance()->isKeyDown(SDL_SCANCODE_LSHIFT) || 
                    Input::getInstance()->isMouseButtonDown(MOUSE_RB)) 
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
        CheckDirectionSetParams(ATK3_FRAME_ROW, ATK3_FRAME_COUNT, ATK3_FRAME_TIME, ATK3_FRAME_START);          //  Attack_3
        if (m_Animation->UpdateSingle())
            m_Condition = Falling;
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

    CheckDirectionSetParams(BLOCK_FRAME_ROW, BLOCK_FRAME_COUNT, BLOCK_FRAME_TIME, BLOCK_FRAME_START);


    if (!Input::getInstance()->isMouseButtonDown(MOUSE_RB))
    {
        m_Condition = Falling;
        return;
    }

    CheckCollisions();
    m_Animation->UpdateCycle();
}




void Knight::Clean()
{
    TextureManager::getInstance()->Drop(m_TextureID);
}





