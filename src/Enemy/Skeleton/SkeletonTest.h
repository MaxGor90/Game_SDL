#ifndef H_SKELETON_20COPY
#define H_SKELETON_20COPY


#include "Enemy.h"


class SkeletonTest : public Enemy
{
public:
    SkeletonTest(std::shared_ptr<ObjParams> params);
    virtual ~SkeletonTest()
    {}

    virtual void Fall(float dt);
    virtual void Attack(float dt);
};


class SkeletonTestFactory : public EnemyFactory
{
public:    
    
    std::shared_ptr<Enemy> Spawn(std::shared_ptr<ObjParams> params) override
    {
        return std::shared_ptr<SkeletonTest>(new SkeletonTest(params));
    }
};



#endif /* H_SKELETON_20COPY */
