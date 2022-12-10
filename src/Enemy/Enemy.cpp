#include "Enemy.h"
#include "SkeletonParams.h"


Enemy::Enemy(std::shared_ptr<ObjParams> params) :
    Character::Character(params)
{
    m_Animation = std::make_unique<Animation>();
    m_CollisionBox = std::make_shared<SDL_Rect>(SDL_Rect{0,0,0,0});
    m_CollisionBoxAtk = std::make_shared<SDL_Rect>(SDL_Rect{0,0,0,0});

    SetParams(m_TextureID, IDLE_FRAME_ROW, IDLE_FRAME_COUNT, IDLE_FRAME_TIME, SDL_FLIP_NONE, IDLE_FRAME_START);
    
    m_RigidBody = std::make_unique<RigidBody>();
    
    m_Collision = Collision::GetInstance();

}


Enemy::~Enemy()
{
    Clean();
}


void Enemy::SetParams(const std::string& textureID, int spriteRow, int frameCount, 
                            int frameTime, SDL_RendererFlip flip, int startFrame)
{
    
}

void Enemy::Draw()
{
    m_Animation->Draw(m_Transform->m_X, m_Transform->m_Y);
}

void Enemy::Update(float dt)
{
    
}

void Enemy::Clean()
{
    
}

