#ifndef H_KNIGHT
#define H_KNIGHT

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collision.h"
#include "Knight_params.h"



class Knight : public Character
{
public:
    Knight(ObjParams* params);
    virtual ~Knight();
    
    virtual void SetParams( const std::string& textureID, int spriteRow, int frameCount, 
                            int frameTime, SDL_RendererFlip flip = SDL_FLIP_NONE, int startFrame = 0);
    
    virtual void Idle(float dt);
    virtual void Run(float dt);
    virtual void Roll(float dt);
    virtual void Jump(float dt);
    virtual void Fall(float dt);
    virtual void Attack(float dt);
    virtual void Block();
    
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Clean();

    void CheckDirectionSetParams(int spriteRow, int frameCount, int frameTime, int startFrame);
    void CheckPosition();
    bool CheckCollisions();
    void PositionRecalc();
    void CollisionBoxRecalc();
    void CollisionBoxAtkRecalc();

protected:

    float m_RunSpeedInFrames {6.0f * 60.0f};    //  Speed in frames so divided by target frame rate speed will be constant on different FRs
    float m_JumpSpeedInFrames{13.0f * 60.0f};

    bool m_IsInAir {true};

    int m_AttackEnds{0};                        //  Time when the last attack ended
    int m_TimeBetweenAttacks{800};              //  Time during which you can continue combo attack on ground
    int m_TimeBetweenAttacksInAir{300};         //  Time during which you can continue combo attack in air
};


#endif /* H_KNIGHT */
