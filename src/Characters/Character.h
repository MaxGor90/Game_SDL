#ifndef H_CHARACTER
#define H_CHARACTER

#include <string>
#include <memory>
#include <unordered_map>
#include "GameObject.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collision.h"
#include "AnimationSequence.h"
#include "AnimationParser.h"



class Character : public GameObject
{
public:
    Character(std::shared_ptr<ObjParams> params);

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
        HIT_1,
        HIT_2,
        HIT_3,
    };


    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;

    void SetDirection(Direction dir) { m_Direction = dir; }

    std::string GetAnimParamsSource();
    void LoadAnimations(const std::string& fileSource);

    virtual void SetParams(std::shared_ptr<AnimationSequence>, SDL_RendererFlip flip = SDL_FLIP_NONE);
    virtual void CollisionBoxRecalc();
    virtual void CheckPosition();
    virtual void CollisionBoxAtkRecalc();
    virtual void PositionRecalc();
    virtual void CheckDirectionSetParams(std::shared_ptr<AnimationSequence> animSeq);
    virtual bool CheckCollisions();

protected:

    std::string m_Name;

    bool m_IsInAir {true};

    std::unique_ptr<Animation> m_Animation {nullptr};
    std::unique_ptr<RigidBody> m_RigidBody {nullptr};
    std::shared_ptr<Collision> m_Collision {nullptr};

    //  Absolute coords and size
    std::shared_ptr<SDL_Rect> m_CollisionBox;
    std::shared_ptr<SDL_Rect> m_CollisionBoxAtk;
    //  Relative to frame coords and size (from m_AnimationSequences)   
    std::shared_ptr<SDL_Rect> m_CollisionBoxInsideFrame;
    std::shared_ptr<SDL_Rect> m_CollisionBoxAtkInsideFrame;

    ComboState m_ComboState {NO};
    Direction m_Direction {forward};
    Condition m_Condition {Falling};

    typedef std::unordered_map<std::string, std::shared_ptr<AnimationSequence>> AnimationsMap;
    AnimationsMap m_AnimationSequences;

    friend class AnimationParser;

};


#endif /* H_CHARACTER */
