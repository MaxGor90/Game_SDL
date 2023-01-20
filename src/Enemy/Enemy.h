#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <map>
#include <string> 

enum enemyType
{
    skeleton_melee = 0,
    skeleton_test
};


// Map with enemy types and their names for parsing EnemyList.xml
static const std::map<std::string, enemyType> EnemyTypeMap{
    {"skeleton_melee", enemyType::skeleton_melee},
    // {"skeleton_test", enemyType::skeleton_test}
};
/*-------------------------------------------------------------*/

class AI;

class Enemy : public Character
{
public:
    Enemy(std::shared_ptr<ObjParams> params);
    virtual ~Enemy();
    
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Clean();

    virtual void Idle(float dt);
    virtual void Run(float dt);
    virtual void Fall(float dt);
    virtual void Roll(float dt)     {dt = dt;};
    virtual void Attack()   {};
    virtual void Block()            {};
    virtual void Jump(float dt)     {dt = dt;};
    virtual void Hurt();

    virtual void SetAggroed()   { isAggroed = true; }
    virtual void SetNotAggroed()   { isAggroed = false; }


protected:
    int m_AttackEnds{0};                //  Time when the last attack ended
    int m_TimeBetweenAttacks{0};      //  Time between attacks in combo
    bool isVulnerable {true};           //  Enemy can take damage 
    
    float m_RunSpeedInFrames;           //  Movement speed for aggroed state
    float m_WalkSpeedInFrames;          //  Movement speed for patrolling (not aggroed) state

    float spawnX, spawnY;               //  Spawn point - for patrolling purpose

    bool isAggroed {false};             //  Aggroed by player - for movement speed 
    
    friend class AI;

    std::shared_ptr<AI> m_AI {nullptr};
};


/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

// Enemy creator interface
class EnemyFactory
{
public:
    virtual ~EnemyFactory() {}

    virtual std::shared_ptr<Enemy> Spawn(std::shared_ptr<ObjParams> params) = 0;
};

// Spawns enemies using EnemyFactory interface
class EnemySpawner
{
    EnemySpawner() {}
    static std::shared_ptr<EnemySpawner> EnemySpawnerInstance;

    //  Map type-factory fot enemies
    std::map<enemyType, std::shared_ptr<EnemyFactory>> m_Factories {};
public:
    static std::shared_ptr<EnemySpawner> getInstance()
    {
        return EnemySpawnerInstance = (EnemySpawnerInstance == nullptr)? std::shared_ptr<EnemySpawner>(new EnemySpawner()) : EnemySpawnerInstance;
    }

    // Parses enemy list and spawns enemies in the passed level
    void SpawnEnemies(const std::string& enemyList, const std::string& levelName);

};

#endif /* ENEMY_H */
