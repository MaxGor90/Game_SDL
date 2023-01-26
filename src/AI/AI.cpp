#include "AI.h"
#include "Character.h"
#include "Timer.h"
#include "Engine.h"
#include "Collision.h"
#include "Battle.h"
#include "Global.h"



AI::AI(BehaviorType behaveType) : Behavior { behaveType }
{
    player = Engine::getInstance().m_Player;
}

void AI::Update(Enemy* enemy) 
{
    if (enemy->GetCondition() == Character::Condition::Attacking || 
        enemy->GetCondition() == Character::Condition::Falling ||
        enemy->GetCondition() == Character::Condition::isHurt)
    {
        return;
    }
    
    ScanAround(enemy);
    
    if (!enemy->isAggroed)
    {
        Patrole(enemy);
        return;
    }

    if (!playerIsInAttackRange(enemy))
    {
        Run(enemy);
        return;
    }
    
    if (!AttackCooldownPassed(enemy))
    {
        // ToDo: defence between attacks
        Idle(enemy);
        return;
    }
    
    Attack(enemy);
}

void AI::Patrole(Enemy* enemy) 
{
    using Condition = Enemy::Condition;
    
    if (enemy->GetCondition() == Condition::IsIdle)
    {
        if (TimePassed(patrollingWaitingTime_ms, true, 0.8, 1.5))
        {
            enemy->SetCondition(Condition::Running);
            enemy->ChangeDirection();
        }
        return;
    }
    
    if (enemy->GetPosition()->X <= enemy->spawnX - patrollingRange || 
        enemy->GetPosition()->X > enemy->spawnX + patrollingRange)
    {
        Idle(enemy);
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
        {
            enemy->SetAggroed();
            Battle::getInstance().addEnemy(enemy);
        }
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
        Battle::getInstance().deleteEnemy(enemy);
        enemy->spawnX = enemyPos;
    }
}



void AI::Run(Enemy* enemy)
{
    enemy->SetCondition(Character::Condition::Running);
}

void AI::Attack(Enemy* enemy)
{
    enemy->SetCondition(Enemy::Condition::Attacking);
    enemy->m_ComboState = Character::ComboState::HIT_1;
}

void AI::Idle(Enemy* enemy)
{
    enemy->SetCondition(Enemy::Condition::IsIdle);
}


bool AI::TimePassed(int time, bool random, float minMultiplier, float maxMultiplier) 
{
    // Seting variables once at the start of waiting cycle
    if (waitingStartTime == 0)
    {
        waitingStartTime = Timer::getInstance().GetLastTime();
        waitingTime = time;
    
        if (random)
            waitingTime = time * minMultiplier + waitingStartTime % (int)(time * (maxMultiplier - minMultiplier));
    }

    // Reseting variables at the end of the waiting cycle
    if (Timer::getInstance().GetLastTime() >= waitingStartTime + waitingTime)
    {
        waitingStartTime = 0;
        waitingTime = 0;
        return true;
    }

    return false;
}

bool AI::AttackCooldownPassed(Enemy* enemy)
{
    return (Timer::getInstance().GetLastTime() >= enemy->m_AttackEnds + attackCooldown_ms);
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
    return Collision::CheckCollision(enemy->m_CollisionBoxAtk, player->GetCollisionBox(), attackOverlapX);
}

