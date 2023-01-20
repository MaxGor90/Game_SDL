#ifndef AI_H
#define AI_H

#include "Enemy.h"

/*----- AI handler-----*/

enum BehaviorType
{
    aggressive =0,
    defensive,
    friendly
};

class AI
{
    BehaviorType Behavior;
    int sightRange;             // Range of aggro by clear sight 
    int hearRange;              // Range of aggro without clear sight ("by ear")
    int patrollingRange {150};  // Range of patrolling (from spawn point) while not aggroed
    int patrollingWaitingTime_ms {3000};    // Base time waiting before changing direction while patrolling (ms)
    int waitingStartTime {0};   // Starting time for waiting 
    int waitingTime {0};        // Actual time time to wait. Is set and reset by TimePassed function
    int chasingRange;           // Area from spawn point to chase player
    int attackCooldown_ms{1000};    // Cooldown for attack action

    std::shared_ptr<Character> player;  


public:
    AI(BehaviorType behaveType); 

    void setSightRange(int range)       { sightRange = range; }
    void setHearRange(int range)        { hearRange = range; }
    void setPatrollingRange(int range)  { patrollingRange = range; }
    void setPatrolWaitTime(int time)    { patrollingWaitingTime_ms = time; }
    void setChasingRange(int range)     { chasingRange = range; }
    void setAttackCooldown(int cooldown)     { attackCooldown_ms = cooldown; }

    void Update(Enemy* enemy);
    void Patrole(Enemy* enemy);
    void ScanAround(Enemy* enemy);
    void Run(Enemy* enemy);
    void Attack(Enemy* enemy);
    void Idle(Enemy* enemy);

    // "Waiting" function
    bool TimePassed(int time, bool random = false, float minMultiplier = 1, float maxMultiplier = 1);
    bool AttackCooldownPassed(Enemy* enemy);
    bool playerIsInSight(Enemy* enemy);
    bool playerIsInAttackRange(Enemy* enemy);



};
#endif /* AI_H */
