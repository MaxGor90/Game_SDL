#ifndef H_AI
#define H_AI

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
    
    // Average time waiting before changing direction while patrolling (ms)
    // Actual time will be randiomized in [0.7, 1.3]*average time    
    float patrollingWaitingTime_ms {3000};
    int startingTime {0};       // Starting time for waiting 
    float waitingTime {0};          // Actual time time to wait   


public:
    AI(BehaviorType behaveType) : Behavior { behaveType }
    {}

    void Update(Enemy* enemy);
    void Patrole(Enemy* enemy);
    bool TimePassed(int time, bool random = false, float minMultiplier = 1, float maxMultiplier = 1);



};
#endif /* H_AI */
