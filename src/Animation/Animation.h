#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include "SDL.h"


class Animation
{
public:

    Animation() {}

    void UpdateCycle();
    bool UpdateSingle(bool repeatLastFrame = false);

    void Draw(float x, float y);
    void SetParams(const std::string& textureID, int spriteRow, int frameCount, int frameTime, SDL_RendererFlip flip = SDL_FLIP_NONE, int startFrame = 0);

    void SetAnimIsOver() 
    {
        m_IsOver = true;
        m_IsRepeating = false; 
    }

    bool IsRepeating() { return m_IsRepeating; }

private:

    int m_SpriteRow, m_FrameTime, m_FrameCount, m_StartFrame, m_SpriteFrame;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;

    int m_FrameShift {0};
    bool m_IsOver {true};               //  Last animation sequence is over and new hasn't started
    bool m_IsRepeating {false};         //  Repeating last frame of animation
};

#endif /* ANIMATION_H */
