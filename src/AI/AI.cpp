#include "AI.h"
#include "Character.h"
#include "Timer.h"
#include "Engine.h"



AI::AI(BehaviorType behaveType) : Behavior { behaveType }
{
    player = Engine::getInstance()->m_Player;
}

void AI::Update(Enemy* enemy) 
{
    ScanAround(enemy);
    if (!enemy->isAggroed)
    {
        Patrole(enemy);
    } else
        SDL_Log("%s: Aggroed!", SDL_FUNCTION);
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

    switch (enemy->m_Direction)
    {
    case Direction::backward:
    {
        if (enemyPos - player->GetPosition()->X <= sightRange &&
            player->GetPosition()->X - enemyPos <= hearRange)
            enemy->SetAggroed();
        else enemy->SetNotAggroed();
        return;
    }
    case Direction::forward:
    {
        if (enemyPos - player->GetPosition()->X <= hearRange &&
            player->GetPosition()->X - enemyPos <= sightRange)
            enemy->SetAggroed();
        else enemy->SetNotAggroed();
        return;
    }
    default:
        return;
    }
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

