#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"
#include "TileLayer.h"
#include <vector>
#include "Transform.h"



class Collision
{
    static Collision* s_CollisionInstance;


    std::vector<TileLayer*> m_CollisionLayers;
    std::vector<TileMap*> m_CollisionMaps;



public:

    Collision();
    ~Collision()
    {
        delete s_CollisionInstance;
    }

    bool CheckCollision(SDL_Rect* rect_a, SDL_Rect* rect_b);
    bool CollisionWithMapX(Transform** pos, SDL_Rect* r);
    bool CollisionWithMapY(Transform** pos, SDL_Rect* r);


    inline static Collision* GetInstance()  {
        return s_CollisionInstance = (s_CollisionInstance == nullptr)? new Collision() : s_CollisionInstance;
    }



};

#endif /* COLLISION_H */
