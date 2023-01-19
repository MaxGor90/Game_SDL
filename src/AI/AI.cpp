#include "AI.h"
#include "Character.h"
#include "Timer.h"
#include "Engine.h"
#include "Collision.h"



AI::AI(BehaviorType behaveType) : Behavior { behaveType }
{
    player = Engine::getInstance()->m_Player;
}

void AI::Update(Enemy* enemy) 
{
    if (enemy->m_Condition == Character::Condition::Attacking || 
        enemy->m_Condition == Character::Condition::Falling)
    {
        return;
    }
    ScanAround(enemy);
    if (enemy->isAggroed)
    {
        SDL_Log("%s: Aggroed!", SDL_FUNCTION);
        if (playerIsInAttackRange(enemy))
        {
            SDL_Log("%s: Attacking!", SDL_FUNCTION);
            Attack(enemy);
            return;
        }

        Charge(enemy);
        return;
    }
    
    Patrole(enemy);
}

void AI::Patrole(Enemy* enemy) 
{
    using Condition = Enemy::Condition;
    
    if (enemy->m_Condition == Condition::IsIdle)
    {
        if (TimePassed(patrollingWaitingTime_ms, true, 0.8, 1.5))
        {
            enemy->m_Condition = Condition::Running;
            enemy->ChangeDirection();
        }
        return;
    }
    
    if (enemy->GetPosition()->X <= enemy->spawnX - patrollingRange || 
        enemy->GetPosition()->X > enemy->spawnX + patrollingRange)
    {
        enemy->m_Condition = Condition::IsIdle;
    }
}

void AI::ScanAround(Enemy* enemy) 
{
    using Direction = Enemy::Direction;
    float enemyPos {enemy->GetPosition()->X};
    float playerPos {player->GetPosition()->X};

    if (!enemy->isAggroed)
    {
        if (playerIsInSight(enemy))
            enemy->SetAggroed();
        return;
    }

    //  Aggroed
    switch (enemy->m_Direction)
    {
    case Direction::backward:
        if (playerPos > enemyPos)
            enemy->ChangeDirection();
        break;
    case Direction::forward:
        if (enemyPos > playerPos)
            enemy->ChangeDirection();
        break;
    }

    if ( (playerPos > enemy->spawnX + chasingRange || playerPos < enemy->spawnX - chasingRange) &&
         (!playerIsInSight(enemy)) )
    {
        enemy->SetNotAggroed();
        enemy->spawnX = enemyPos;
    }
}



void AI::Charge(Enemy* enemy)
{
    enemy->m_Condition = Enemy::Condition::Running;
}

void AI::Attack(Enemy* enemy)
{
    enemy->m_Condition = Enemy::Condition::Attacking;
    enemy->m_ComboState = Character::ComboState::HIT_1;
}


bool AI::TimePassed(int time, bool random, float minMultiplier, float maxMultiplier) 
{
    // Seting variables once at the start of waiting cycle
    if (startingTime == 0)
    {
        startingTime = Timer::getInstance()->GetLastTime();
        waitingTime = time;
    
        if (random)
            waitingTime = time * minMultiplier + startingTime % (int)(time * (maxMultiplier - minMultiplier));
    }

    // Reseting variables at the end of the waiting cycle
    if (Timer::getInstance()->GetLastTime() >= startingTime + waitingTime)
    {
        startingTime = 0;
        waitingTime = 0;
        return true;
    }

    return false;
}

bool AI::playerIsInSight(Enemy* enemy)
{
    using Direction = Enemy::Direction;
    float enemyPos {enemy->GetPosition()->X};
    float playerPos {player->GetPosition()->X};

    switch (enemy->m_Direction)
    {
    case Direction::backward:
    {
        if (enemyPos - playerPos <= sightRange &&
            enemyPos > playerPos)
        {
            return true;
        }
        if (playerPos - enemyPos <= hearRange && 
            enemyPos < playerPos)
        {
            enemy->ChangeDirection();
            return true;
        }
        return false;
    }
    case Direction::forward:
    {
        if (enemyPos - playerPos <= hearRange &&
            enemyPos > playerPos)
        {
            enemy->ChangeDirection();
            return true;
        }
        if (playerPos - enemyPos <= sightRange &&
            enemyPos < playerPos)
        {
            return true;
        }
        return false;
    }
    default:
        return false;
    }
}

bool AI::playerIsInAttackRange(Enemy* enemy)
{
    enemy->CollisionBoxAtkRecalc();
    return Collision::CheckCollision(enemy->m_CollisionBoxAtk, player->GetCollisionBox());
}

