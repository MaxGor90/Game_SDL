#ifndef H_ENEMY
#define H_ENEMY

#include "Character.h"



class Enemy : public Character
{
public:
    Enemy(ObjParams* params);
    virtual ~Enemy();
    
    virtual void SetParams( const std::string& textureID, int spriteRow, int frameCount, 
                            int frameTime, SDL_RendererFlip flip = SDL_FLIP_NONE, int startFrame = 0);
    
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Clean();




}; 



#endif /* H_ENEMY */
