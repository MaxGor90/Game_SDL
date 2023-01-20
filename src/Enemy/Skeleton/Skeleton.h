#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"


class Skeleton : public Enemy
{
public:
    Skeleton(std::shared_ptr<ObjParams> params);
    virtual ~Skeleton()
    {}

    virtual void Attack();
};


class SkeletonFactory : public EnemyFactory
{
public:    
    
    std::shared_ptr<Enemy> Spawn(std::shared_ptr<ObjParams> params) override
    {
        return std::shared_ptr<Skeleton>(new Skeleton(params));
    }
};

#endif /* SKELETON_H */
