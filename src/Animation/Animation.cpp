#include "Animation.h"
#include "TextureManager.h"
#include "SDL.h"



//  Cyclic animation like running or idle - starts with "random" time-depending frame from m_StartFrame till m_StartFrame + m_FrameCount
void Animation::UpdateCycle()
{   
    m_SpriteFrame = (static_cast<int>(SDL_GetTicks() / m_FrameTime) % m_FrameCount) + m_StartFrame; // Frames count from 0
}


//   Animation of single action
//   Starts exactly with m_StartFrame and goes till m_StartFrame + m_FrameCount
bool Animation::UpdateSingle(bool repeatLastFrame)
{
    if (m_IsOver) 
    {
        //  "random" time-depending frame shift which we need to negate to start animation from 0 frame
        //  calculates ones at the start of each Single animation       
        m_FrameShift = static_cast<int>(SDL_GetTicks64() / m_FrameTime);
    }
    
    if (!m_IsRepeating)         
    {
        m_IsOver = false;

        //  Negating frame shift by substracting it from overall frames before calculating division remainder
        m_SpriteFrame = (static_cast<int>(SDL_GetTicks64() / m_FrameTime  - m_FrameShift) % m_FrameCount) + m_StartFrame;

        if (m_SpriteFrame == m_StartFrame + m_FrameCount -1)        //  Got to last frame in sequence
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
        m_SpriteFrame = m_StartFrame + m_FrameCount -1;

        
    return false;
}


void Animation::Draw(float x, float y)
{
    TextureManager::getInstance()->DrawFrame(m_TextureID, x, y, m_SpriteRow, m_SpriteFrame, m_Flip);
}

void Animation::SetParams(const std::string& textureID, int spriteRow, int frameCount, int frameTime, SDL_RendererFlip flip, int startFrame)
{
    m_TextureID = textureID;
    m_SpriteRow = spriteRow;
    m_FrameCount = frameCount;
    m_FrameTime = frameTime;
    m_StartFrame = startFrame;
    m_Flip = flip;
}
