#ifndef H_SKELETON
#define H_SKELETON

#include "Enemy.h"


class Skeleton : public Enemy
{
public:
    Skeleton(std::shared_ptr<ObjParams> params);
    virtual ~Skeleton()
    {}

    virtual void Fall(float dt);
    virtual void Attack(float dt);
};


class SkeletonFactory : public EnemyFactory
{
public:    
    
    std::shared_ptr<Enemy> Spawn(std::shared_ptr<ObjParams> params) override
    {
        return std::shared_ptr<Skeleton>(new Skeleton(params));
    }
};

#endif /* H_SKELETON */
