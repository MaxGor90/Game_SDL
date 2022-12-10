#ifndef H_CHARACTER
#define H_CHARACTER

#include <string>
#include "GameObject.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collision.h"
#include <memory>


class Character : public GameObject
{
public:
    Character(std::shared_ptr<ObjParams> params) :
        GameObject(params)
    {}

    virtual ~Character()
    {}

    enum Direction {
        forward = 0,
        backward
    };
    
    enum Condition {
        IsIdle,
        Running, 
        Rolling,
        Jumping,
        Falling,
        Attacking,
        Blocking
    };

    enum ComboState {
        NO = 0,
        HIT_1 = 1,
        HIT_2,
        HIT_3,
    };


    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;

    void SetDirection(Direction dir) { m_Direction = dir; }

protected:

    std::string m_Name;

    bool m_IsInAir {true};

    std::unique_ptr<Animation> m_Animation {nullptr};
    std::unique_ptr<RigidBody> m_RigidBody {nullptr};
    std::shared_ptr<Collision> m_Collision {nullptr};

    std::shared_ptr<SDL_Rect> m_CollisionBox{nullptr};
    std::shared_ptr<SDL_Rect> m_CollisionBoxAtk{nullptr};

    ComboState m_ComboState {NO};
    Direction m_Direction {forward};
    Condition m_Condition {Falling};

};

#endif /* H_CHARACTER */
