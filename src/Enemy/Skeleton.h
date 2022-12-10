#ifndef H_SKELETON
#define H_SKELETON


#include "Enemy.h"


class Skeleton : public Enemy
{
public:
    Skeleton(std::shared_ptr<ObjParams> params);
    virtual ~Skeleton();

    


};

#endif /* H_SKELETON */
