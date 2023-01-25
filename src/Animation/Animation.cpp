#include "Animation.h"
#include "TextureManager.h"
#include "Timer.h"
#include "SDL.h"




//  Cyclic animation like running or idle - starts with "random" time-depending frame from m_StartFrame till m_StartFrame + m_FrameCount
void Animation::UpdateCycle()
{   
    m_SpriteFrame = (static_cast<int>(Timer::getInstance().GetLastTime() / m_AnimSequence->params->frameTime) % m_AnimSequence->params->frameCount) + m_AnimSequence->params->startFrame; // Frames count from 0
}


//   Animation of single action
//   Starts exactly with m_AnimSequence->params->startFrame and goes till m_AnimSequence->params->startFrame + m_AnimSequence->params->frameCount
bool Animation::UpdateSingle(bool repeatLastFrame)
{
    if (m_IsOver) 
    {
        //  "random" time-depending frame shift which we need to negate to start animation from 0 frame
        //  calculates ones at the start of each Single animation       
        m_FrameShift = static_cast<int>(Timer::getInstance().GetLastTime() / m_AnimSequence->params->frameTime);
    }
    
    if (!m_IsRepeating)         
    {
        m_IsOver = false;

        //  Negating frame shift by substracting it from overall frames before calculating division remainder
        m_SpriteFrame = (static_cast<int>(Timer::getInstance().GetLastTime() / m_AnimSequence->params->frameTime  - m_FrameShift) % m_AnimSequence->params->frameCount) + m_AnimSequence->params->startFrame;

        if (m_SpriteFrame == m_AnimSequence->params->startFrame + m_AnimSequence->params->frameCount -1)        //  Got to last frame in sequence
        {
            if (repeatLastFrame)
            {
                m_IsRepeating = true;
                return true;
            }
            else 
            {    
                m_IsOver = true;
                return true;
            }
        }
    }

    if (m_IsRepeating)              //  Repeating last frame
        m_SpriteFrame = m_AnimSequence->params->startFrame + m_AnimSequence->params->frameCount -1;

        
    return false;
}


void Animation::Draw(float x, float y)
{
    TextureManager::getInstance().DrawFrame(m_TextureID, x, y, m_AnimSequence->params->row, m_SpriteFrame, m_Flip);
}

void Animation::SetParams(const std::string& textureID, std::shared_ptr<AnimationSequence> animSequence, SDL_RendererFlip flip)
{
    m_TextureID = textureID;
    if (m_AnimSequence != animSequence)
    {
        SetAnimIsOver();
        m_AnimSequence = animSequence;
    }
    m_Flip = flip;
}

bool Animation::IsHitFrame()
{
    return (m_SpriteFrame >= m_AnimSequence->getHitFrameStart() && 
            m_SpriteFrame < m_AnimSequence->getHitFrameStart() + m_AnimSequence->getHitFrames());
}
