#ifndef H_ANIMATION
#define H_ANIMATION

#include <string>
#include "SDL.h"
#include "AnimationSequence.h"


class Animation
{
public:

    Animation() {}

    void UpdateCycle();
    bool UpdateSingle(bool repeatLastFrame = false);

    void Draw(float x, float y);
    void SetParams(const std::string& textureID, std::shared_ptr<AnimationSequence> animSequence, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void SetAnimIsOver() 
    {
        m_IsOver = true;
        m_IsRepeating = false; 
    }

    bool IsRepeating() { return m_IsRepeating; }

private:

    std::shared_ptr<AnimationSequence> m_AnimSequence {nullptr};
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;

    int m_SpriteFrame;
    int m_FrameShift {0};
    bool m_IsOver {true};               //  Last animation sequence is over and new hasn't started
    bool m_IsRepeating {false};         //  Repeating last frame of animation
};

#endif /* H_ANIMATION */
