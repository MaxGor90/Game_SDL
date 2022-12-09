#include "Timer.h"
#include "SDL.h"


Timer* Timer::s_TimerInstance {nullptr};


void Timer::Tick()
{
    m_DeltaTime = ((float)SDL_GetTicks64() - m_LastTime) * ((float)TARGET_FPS / 1000.0f); // DeltaTime in frames with TARGET_FPS

    if ( m_DeltaTime > TARGET_DELTATIME)
    {
        m_DeltaTime = TARGET_DELTATIME;
    }

    // SDL_Log("%s : dt = %f, lastTime = %f", SDL_FUNCTION, m_DeltaTime, m_LastTime);

    m_LastTime = (float)SDL_GetTicks64();

}



