#ifndef CHARACTER_H
#define CHARACTER_H

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
        backward = -1,
        forward = 1
    };
    
    enum Condition {
        IsIdle,
        Running, 
        Rolling,
        Jumping,
        Falling,
        Attacking,
        Blocking,
        isHurt
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
    void ChangeDirection();

    std::string GetAnimParamsSource();
    void LoadAnimations(const std::string& fileSource);

    virtual void SetParams(std::shared_ptr<AnimationSequence>, SDL_RendererFlip flip = SDL_FLIP_NONE);
    virtual void CollisionBoxRecalc();
    virtual void CheckPosition();
    virtual void CollisionBoxAtkRecalc();
    virtual void PositionRecalc();
    virtual void CheckDirectionSetParams(std::shared_ptr<AnimationSequence> animSeq);
    virtual bool CheckCollisions();

    virtual std::shared_ptr<SDL_Rect> GetCollisionBox() { return m_CollisionBox; }
    virtual std::shared_ptr<SDL_Rect> GetCollisionBoxAtk() { return m_CollisionBoxAtk; }
    virtual Condition GetCondition() { return m_Condition; }
    virtual void SetCondition(Condition condition) { m_Condition = condition; }

    virtual void SetHitScored(bool scored) { m_HitScored = scored; }
    virtual bool GetHitScored() { return m_HitScored; }
    virtual bool IsHittingFrames() { return m_Animation->IsHitFrame(); }
    

protected:

    bool m_IsInAir {true};

    std::unique_ptr<Animation> m_Animation {nullptr};
    std::unique_ptr<RigidBody> m_RigidBody {nullptr};
    Collision& m_Collision { Collision::getInstance() };

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

    bool m_HitScored {false};     // Current attack action (Hit) already scored a hit
};


#endif /* CHARACTER_H */
