#ifndef H_ENEMY
#define H_ENEMY

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
    {"skeleton_test", enemyType::skeleton_test}
};
/*-------------------------------------------------------------*/


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
    virtual void Fall(float dt)     {dt = dt;};
    virtual void Roll(float dt)     {dt = dt;};
    virtual void Attack(float dt)   {dt = dt;};
    virtual void Block()            {};
    virtual void Jump(float dt)     {dt = dt;};
};

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

#endif /* H_ENEMY */
