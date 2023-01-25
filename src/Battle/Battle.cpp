#include "Battle.h"
#include "Engine.h"
#include "Collision.h"
#include "Global.h"


Battle::Battle()
{
    Player = Engine::getInstance().getPalyer();
}


void Battle::addEnemy(Enemy* enemy)
{
    if (EnemyList.find(enemy) != EnemyList.end())
        return;
    EnemyList.insert(enemy);
}

void Battle::deleteEnemy(Enemy* enemy)
{
    if (EnemyList.find(enemy) == EnemyList.end())
        return;
    EnemyList.erase(enemy);
}

void Battle::Update()
{
    for (auto enemy : EnemyList)
    {
        CheckPlayerIsHit(enemy);
        CheckEnemyIsHit(enemy);
    }
}

void Battle::CheckPlayerIsHit(Enemy* enemy)
{
    if (!enemy->IsHittingFrames())
        return;
    if (enemy->GetHitScored())
        return;
    if (Player->GetCondition() == Character::Condition::Blocking)
        return;
    if (Collision::CheckCollision(Player->GetCollisionBox(), enemy->GetCollisionBoxAtk(), attackOverlapX))
    {
        Player->SetCondition(Character::Condition::isHurt);
        enemy->SetHitScored(true);
        return;
    }
    
    return;
}

void Battle::CheckEnemyIsHit(Enemy* enemy)
{
    if (!Player->IsHittingFrames())
        return;
    if (Player->GetHitScored())
        return;
    if (Collision::CheckCollision(Player->GetCollisionBox(), enemy->GetCollisionBoxAtk(), attackOverlapX))
    {
        enemy->SetCondition(Character::Condition::isHurt);
        Player->SetHitScored(true);
        return;
    }

    return;
}