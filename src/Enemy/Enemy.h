#ifndef H_ENEMY
#define H_ENEMY

#include "Character.h"



class Enemy : public Character
{
public:
    Enemy(std::shared_ptr<ObjParams> params);
    virtual ~Enemy();
    
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Clean();

}; 



#endif /* H_ENEMY */
