#ifndef H_ANIMATIONSEQUENCE
#define H_ANIMATIONSEQUENCE

#include <string>
#include <memory>
#include "SDL.h"


enum animType
{
    // Animation type not requiring additional collision box
    basic = 0,
    // Animation type requiring additional collision box
    attack
};

// Animation parameters for basic animation type
struct AnimationParams
{
    std::string name;
    int row;
    int frameCount;
    int startFrame;
    int frameTime;
    std::shared_ptr<SDL_Rect> collisionBox;
};


class AnimationSequence
{
public:

    std::shared_ptr<AnimationParams> params;

    animType  animationType;
    AnimationSequence( animType type, const std::string& Name, int Row, int frCount, int startFr, int frTime, std::shared_ptr<SDL_Rect> colBox) :
        params { std::make_shared<AnimationParams>( AnimationParams{Name,
                                                    Row,
                                                    frCount,
                                                    startFr,
                                                    frTime,
                                                    colBox} ) },
        animationType {type}
    {}

    virtual const std::string& getName() { return params->name; }
    virtual std::shared_ptr<SDL_Rect> getCollisionBox() { return params->collisionBox; }
    virtual std::shared_ptr<SDL_Rect> getAttackCollisionBox() { return nullptr; }

};

class AttackAnimationSequence : public AnimationSequence
{
public:
    std::shared_ptr<SDL_Rect> attackCollisionBox;

    AttackAnimationSequence( animType type, const std::string& Name, int Row, int frCount, int startFr, int frTime, std::shared_ptr<SDL_Rect> colBox,
                            std::shared_ptr<SDL_Rect> atkColBox ) :
        AnimationSequence( type, Name, Row, frCount, startFr, frTime, colBox),
        attackCollisionBox {atkColBox}
    {}

    std::shared_ptr<SDL_Rect> getAttackCollisionBox() override 
    {   return attackCollisionBox; }
};


#endif /* H_ANIMATIONSEQUENCE */
