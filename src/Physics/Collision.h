#ifndef H_COLLISION
#define H_COLLISION

#include "SDL.h"
#include "TileLayer.h"
#include <vector>
#include "Transform.h"
#include <memory>



class Collision
{
    static std::shared_ptr<Collision> s_CollisionInstance;


    std::vector<std::shared_ptr<TileLayer>> m_CollisionLayers;
    std::vector<std::shared_ptr<TileMap>> m_CollisionMaps;



public:

    Collision();
    ~Collision()
    {}

    bool CheckCollision(std::shared_ptr<SDL_Rect> collisionBox1, std::shared_ptr<SDL_Rect> collisionBox2);
    bool CollisionWithMapX(std::shared_ptr<Transform>* position, std::shared_ptr<SDL_Rect> collisionBox);
    bool CollisionWithMapY(std::shared_ptr<Transform>* position, std::shared_ptr<SDL_Rect> collisionBox);


    inline static std::shared_ptr<Collision> GetInstance()  {
        return s_CollisionInstance = (s_CollisionInstance == nullptr)? std::make_shared<Collision>() : s_CollisionInstance;
    }



};

#endif /* H_COLLISION */
