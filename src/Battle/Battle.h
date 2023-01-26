#ifndef BATTLE_H
#define BATTLE_H

#include <memory>
#include <set>
#include "Character.h"
#include "Enemy.h"

template<typename T>
using Ptr = std::shared_ptr<T>;

class Battle
{
    Battle();
    ~Battle() {}
    
    Ptr<Character> Player{nullptr};
    std::set<Enemy*> EnemyList;


    friend class shared_ptr;

public:
    static Battle& getInstance()
    {
        static Battle BattleInstance;
        return BattleInstance;
    }

    void addEnemy(Enemy* enemy);

    void deleteEnemy(Enemy* enemy);

    void Update();
    void CheckPlayerIsHit(Enemy* enemy);
    void CheckEnemyIsHit(Enemy* enemy);
    bool CheckPlayerFacesEnemy(Enemy* enemy);

};
#endif /* BATTLE_H */
