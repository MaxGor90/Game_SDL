#include "Enemy.h"


Enemy::Enemy(std::shared_ptr<ObjParams> params) :
    Character::Character(params)
{
    m_Animation = std::make_unique<Animation>();

    m_RigidBody = std::make_unique<RigidBody>();
    
    m_Collision = Collision::GetInstance();
}


Enemy::~Enemy()
{
    Clean();
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
