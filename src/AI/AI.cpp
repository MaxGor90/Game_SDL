#include "AI.h"
#include "Character.h"
#include "Timer.h"



void AI::Update(Enemy* enemy) 
{
    if (!enemy->isAggroed)
    {
        Patrole(enemy);
    }
}

void AI::Patrole(Enemy* enemy) 
{
    typedef Enemy::Condition Condition;
    
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
